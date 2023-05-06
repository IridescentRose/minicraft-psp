/*
 * LevelGen.cpp
 *
 *  Created on: 17 ���. 2013 �.
 *      Author: user
 */

#include "LevelGen.h"
#include <oslib/oslib.h>

Random* LevelGen::random = new Random();

LevelGen::~LevelGen()
{
	delete[] values;
}

LevelGen::LevelGen(int w, int h, int featureSize)
{
	this->w = w;
	this->h = h;
	widthMinusOne = w - 1;
	heightMinusOne = h - 1;

	values = new float[w * h];

	for (int y = 0; y < w; y += featureSize) {
		for (int x = 0; x < w; x += featureSize) {
			setSample(x, y, random->nextFloat() * 2 - 1);
		}
	}

	int stepSize = featureSize;
	float scale = 1.0 / w;
	float scaleMod = 1;
	do {
		int halfStep = stepSize / 2;
		float stepSizeByScale = stepSize * scale;
		for (int y = 0; y < w; y += stepSize) {
			for (int x = 0; x < w; x += stepSize) {
				float a = sample(x, y);
				float b = sample(x + stepSize, y);
				float c = sample(x, y + stepSize);
				float d = sample(x + stepSize, y + stepSize);

				float e = (a + b + c + d) / 4.0
						+ (random->nextFloat() * 2 - 1) * stepSizeByScale;
				setSample(x + halfStep, y + halfStep, e);
			}
		}
		stepSizeByScale /= 2;	//same as multiply by 0.5 needed for next steps
		for (int y = 0; y < w; y += stepSize) {
			for (int x = 0; x < w; x += stepSize) {
				float a = sample(x, y);
				float b = sample(x + stepSize, y);
				float c = sample(x, y + stepSize);
				float d = sample(x + halfStep, y + halfStep);
				float e = sample(x + halfStep, y - halfStep);
				float f = sample(x - halfStep, y + halfStep);

				float H = (a + b + d + e) / 4.0
						+ (random->nextFloat() * 2 - 1) * stepSizeByScale;
				float g = (a + c + d + f) / 4.0
						+ (random->nextFloat() * 2 - 1) * stepSizeByScale;
				setSample(x + halfStep, y, H);
				setSample(x, y + halfStep, g);
			}
		}
		stepSize /= 2;
		scale *= (scaleMod + 0.8);
		scaleMod *= 0.3;
	} while (stepSize > 1);
}

float LevelGen::sample(int x, int y)
{
	return values[(x & widthMinusOne) + (y & heightMinusOne) * w];
}

void LevelGen::setSample(int x, int y, float value)
{
	values[(x & widthMinusOne) + (y & heightMinusOne) * w] = value;
}

uint8_t ** LevelGen::createAndValidateTopMap(int w, int h)
{
	int attempt = 0;
	uint8_t ** result = NULL;
	do
	{
		if (result)
		{
			delete result[0];
			delete result[1];
			delete[] result;
		}
		result = createTopMap(w, h);
		//oslDebug("wehave created the map!!");
		//oslPrintf("attempt %d",attempt++);
		int count[256];

		for (int i = 0; i < w * h; i++)
		{
			count[result[0][i] & 0xff]++;
		}
		if (count[Tile::rock->id & 0xff] < 100)
		{
//			oslDebug("rock id %d",Tile::rock->id);
			continue;
		}
		if (count[Tile::sand->id & 0xff] < 100)
		{
//			oslDebug("sand id %d",Tile::sand->id);
			continue;
		}
		if (count[Tile::grass->id & 0xff] < 100)
		{
//			oslDebug("grass id %d",Tile::grass->id);
			continue;
		}
		if (count[Tile::tree->id & 0xff] < 100)
		{
//			oslDebug("tree id %d",Tile::tree->id);
			continue;
		}
		if (count[Tile::stairsDown->id & 0xff] < 2)
		{
			continue;
		}

		return result;

	} while (true);
	//oslBenchmarkTest(OSL_BENCH_END);
	//oslDebug("%d",oslBenchmarkTest(OSL_BENCH_GET));
}

uint8_t ** LevelGen::createAndValidateUndergroundMap(int w, int h, int depth)
{
	int attempt = 0;
	uint8_t ** result = NULL;
	do
	{
		if (result)
		{
			delete result[0];
			delete result[1];
			delete[] result;
		}

		result = createUndergroundMap(w, h, depth);

		int count[256];

		for (int i = 0; i < w * h; i++)
		{
			count[result[0][i] & 0xff]++;
		}
		if (count[Tile::rock->id & 0xff] < 100) continue;
		if (count[Tile::dirt->id & 0xff] < 100) continue;
		if (count[(Tile::ironOre->id & 0xff) + depth - 1] < 20) continue;
		if (depth < 3) if (count[Tile::stairsDown->id & 0xff] < 2) continue;

		return result;

	} while (true);
}

uint8_t ** LevelGen::createAndValidateSkyMap(int w, int h)
{
	int attempt = 0;
	uint8_t ** result = NULL;
	do
	{
		if (result)
		{
			delete result[0];
			delete result[1];
			delete[] result;
		}
		result = createSkyMap(w, h);

		int count[256];

		for (int i = 0; i < w * h; i++) {
			count[result[0][i] & 0xff]++;
		}
		if (count[Tile::cloud->id & 0xff] < 2000) continue;
		if (count[Tile::stairsDown->id & 0xff] < 2) continue;

		return result;

	} while (true);
}

uint8_t ** LevelGen::createTopMap(int w, int h)
{
	LevelGen mnoise1(w, h, 16);
	LevelGen mnoise2(w, h, 16);
	LevelGen mnoise3(w, h, 16);

	LevelGen noise1(w, h, 32);
	LevelGen noise2(w, h, 32);


	uint8_t * map = new uint8_t[w * h];
	uint8_t * data = new uint8_t[w * h];
	memset(data, 0 ,w * h * sizeof(uint8_t));
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int i = x + y * w;

			float val = oslAbs(noise1.values[i] - noise2.values[i]) * 3 - 2;
			float mval = oslAbs(mnoise1.values[i] - mnoise2.values[i]);
			mval = oslAbs(mval - mnoise3.values[i]) * 3 - 2;

			float xd = x / (w - 1.0) * 2 - 1;
			float yd = y / (h - 1.0) * 2 - 1;
			if (xd < 0)
				xd = -xd;
			if (yd < 0)
				yd = -yd;
			float dist = xd >= yd ? xd : yd;
			if (dist != 1.0)
			{
				dist *= dist;
				dist *= dist;
				dist *= dist;
				dist *= dist;
			}

			val = val + 1 - dist * 20;

			if (val < -0.5) {
				map[i] = Tile::water->id;
			} else if (val > 0.5 && mval < -1.5) {
				map[i] = Tile::rock->id;
			} else {
				map[i] = Tile::grass->id;
			}
		}
	}

	for (int i = 0; i < w * h / 2800; i++) {
		int xs = random->nextInt(w);
		int ys = random->nextInt(h);
		for (int k = 0; k < 10; k++) {
			int x = xs + random->nextInt(21) - 10;
			int y = ys + random->nextInt(21) - 10;
			for (int j = 0; j < 100; j++) {
				int xo = x + random->nextInt(5) - random->nextInt(5);
				int yo = y + random->nextInt(5) - random->nextInt(5);
				for (int yy = yo - 1; yy <= yo + 1; yy++)
					for (int xx = xo - 1; xx <= xo + 1; xx++)
						if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
							if (map[xx + yy * w] == Tile::grass->id)
							{
								map[xx + yy * w] = Tile::sand->id;
							}
						}
			}
		}
	}

	/*
	 * for (int i = 0; i < w * h / 2800; i++) { int xs = random->nextInt(w); int ys = random->nextInt(h); for (int k = 0; k < 10; k++) { int x = xs + random->nextInt(21) - 10; int y = ys + random->nextInt(21) - 10; for (int j = 0; j < 100; j++) { int xo = x + random->nextInt(5) - random->nextInt(5); int yo = y + random->nextInt(5) - random->nextInt(5); for (int yy = yo - 1; yy <= yo + 1; yy++) for (int xx = xo - 1; xx <= xo + 1; xx++) if (xx >= 0 && yy >= 0 && xx < w && yy < h) { if (map[xx + yy * w] == Tile::grass->id) { map[xx + yy * w] = Tile::dirt->id; } } } } }
	 */

	for (int i = 0; i < w * h / 400; i++) {
		int x = random->nextInt(w);
		int y = random->nextInt(h);
		for (int j = 0; j < 200; j++) {
			int xx = x + random->nextInt(15) - random->nextInt(15);
			int yy = y + random->nextInt(15) - random->nextInt(15);
			if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				if (map[xx + yy * w] == Tile::grass->id) {
					map[xx + yy * w] = Tile::tree->id;
				}
			}
		}
	}

	for (int i = 0; i < w * h / 400; i++) {
		int x = random->nextInt(w);
		int y = random->nextInt(h);
		int col = random->nextInt(4);
		for (int j = 0; j < 30; j++) {
			int xx = x + random->nextInt(5) - random->nextInt(5);
			int yy = y + random->nextInt(5) - random->nextInt(5);
			if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
				if (map[xx + yy * w] == Tile::grass->id) {
					map[xx + yy * w] = Tile::flower->id;
					data[xx + yy * w] =
							(uint8_t) (col + random->nextInt(4) * 16);
				}
			}
		}
	}

	for (int i = 0; i < w * h / 100; i++) {
		int xx = random->nextInt(w);
		int yy = random->nextInt(h);
		if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
			if (map[xx + yy * w] == Tile::sand->id) {
				map[xx + yy * w] = Tile::cactus->id;
			}
		}
	}

	int count = 0;
	int i = 0;
	int repeats = w * h / 100;
	while (i < repeats)
	{
		int x = random->nextInt(w - 2) + 1;
		int y = random->nextInt(h - 2) + 1;

		for (int yy = y - 1; yy <= y + 1; yy++)
			for (int xx = x - 1; xx <= x + 1; xx++)
			{
				if (map[xx + yy * w] != Tile::rock->id)
				{
					goto stairsLoop;
				}
			}

		map[x + y * w] = Tile::stairsDown->id;
		count++;
		if (count == 4)
			break;
		stairsLoop:
		i++;
	}

	uint8_t ** result = new uint8_t*[2];
	result[0] = map;
	result[1] = data;
	return result;
}

uint8_t ** LevelGen::createUndergroundMap(int w, int h, int depth)
{
	LevelGen mnoise1(w, h, 16);
	LevelGen mnoise2(w, h, 16);
	LevelGen mnoise3(w, h, 16);

	LevelGen nnoise1(w, h, 16);
	LevelGen nnoise2(w, h, 16);
	LevelGen nnoise3(w, h, 16);

	LevelGen wnoise1(w, h, 16);
	LevelGen wnoise2(w, h, 16);
	LevelGen wnoise3(w, h, 16);

	LevelGen noise1(w, h, 32);
	LevelGen noise2(w, h, 32);

	uint8_t * map = new uint8_t[w * h];
	uint8_t * data = new uint8_t[w * h];
	memset(data, 0 ,w * h * sizeof(uint8_t));
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = x + y * w;

			float val = oslAbs(noise1.values[i] - noise2.values[i]) * 3 - 2;

			float mval = oslAbs(mnoise1.values[i] - mnoise2.values[i]);
			mval = oslAbs(mval - mnoise3.values[i]) * 3 - 2;

			float nval = oslAbs(nnoise1.values[i] - nnoise2.values[i]);
			nval = oslAbs(nval - nnoise3.values[i]) * 3 - 2;

			float wval = oslAbs(wnoise1.values[i] - wnoise2.values[i]);
			wval = oslAbs(nval - wnoise3.values[i]) * 3 - 2;

			float xd = x / (w - 1.0) * 2 - 1;
			float yd = y / (h - 1.0) * 2 - 1;
			if (xd < 0) xd = -xd;
			if (yd < 0) yd = -yd;
			float dist = xd >= yd ? xd : yd;
			if (dist != 1.0)
			{
				dist *= dist;
				dist *= dist;
				dist *= dist;
				dist *= dist;
			}

			val = val + 1 - dist * 20;

			if (val > -2 && wval < -2.0 + (depth) / 2 * 3) {
				if (depth > 2)
					map[i] = Tile::lava->id;
				else
					map[i] = Tile::water->id;
			} else if (val > -2 && (mval < -1.7 || nval < -1.4)) {
				map[i] = Tile::dirt->id;
			} else {
				map[i] = Tile::rock->id;
			}
		}
	}

	{
		int r = 2;
		for (int i = 0; i < w * h / 400; i++) {
			int x = random->nextInt(w);
			int y = random->nextInt(h);
			for (int j = 0; j < 30; j++) {
				int xx = x + random->nextInt(5) - random->nextInt(5);
				int yy = y + random->nextInt(5) - random->nextInt(5);
				if (xx >= r && yy >= r && xx < w - r && yy < h - r) {
					if (map[xx + yy * w] == Tile::rock->id) {
						map[xx + yy * w] = (uint8_t) ((Tile::ironOre->id & 0xff) + depth - 1);
					}
				}
			}
		}
	}

	if (depth < 3)
	{
		int count = 0;
		int i = 0;
		int repeats = w * h / 100;
		while (i < repeats)
		{
			int x = random->nextInt(w - 2) + 1;
			int y = random->nextInt(h - 2) + 1;

			for (int yy = y - 1; yy <= y + 1; yy++)
				for (int xx = x - 1; xx <= x + 1; xx++)
				{
					if (map[xx + yy * w] != Tile::rock->id)
					{
						goto stairsLoop;
					}
				}

			map[x + y * w] = Tile::stairsDown->id;
			count++;
			if (count == 4)
				break;
			stairsLoop:
			i++;
		}
	}

	uint8_t ** result = new uint8_t*[2];
	result[0] = map;
	result[1] = data;
	return result;
}

uint8_t ** LevelGen::createSkyMap(int w, int h)
{
	LevelGen noise1(w, h, 8);
	LevelGen noise2(w, h, 8);

	uint8_t * map = new uint8_t[w * h];
	uint8_t * data = new uint8_t[w * h];
	memset(data, 0 ,w * h * sizeof(uint8_t));
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int i = x + y * w;

			float val = oslAbs(noise1.values[i] - noise2.values[i]) * 3 - 2;

			float xd = x / (w - 1.0) * 2 - 1;
			float yd = y / (h - 1.0) * 2 - 1;
			if (xd < 0) xd = -xd;
			if (yd < 0) yd = -yd;
			float dist = xd >= yd ? xd : yd;
			if (dist != 1.0)
			{
				dist *= dist;
				dist *= dist;
				dist *= dist;
				dist *= dist;
			}
			val = -val * 1 - 2.2;
			val = val + 1 - dist * 20;

			if (val < -0.25) {
				map[i] = Tile::infiniteFall->id;
			} else {
				map[i] = Tile::cloud->id;
			}
		}
	}


	int count = 0;
	int i = 0;
	int repeats = w * h / 50;
	while (i < repeats)
	{
		int x = random->nextInt(w - 2) + 1;
		int y = random->nextInt(h - 2) + 1;

		for (int yy = y - 1; yy <= y + 1; yy++)
			for (int xx = x - 1; xx <= x + 1; xx++)
			{
				if (map[xx + yy * w] != Tile::cloud->id)
				{
					goto cloudCactusesLoop;
				}
			}

		map[x + y * w] = Tile::cloudCactus->id;
		//count++;
		//if (count == 4)
		//	break;
		cloudCactusesLoop:
		i++;
	}


	count = 0;
	i = 0;
	repeats = w * h;
	while (i < repeats)
	{
		int x = random->nextInt(w - 2) + 1;
		int y = random->nextInt(h - 2) + 1;

		for (int yy = y - 1; yy <= y + 1; yy++)
			for (int xx = x - 1; xx <= x + 1; xx++)
			{
				if (map[xx + yy * w] != Tile::cloud->id)
				{
					goto stairsLoop;
				}
			}

		map[x + y * w] = Tile::stairsDown->id;
		count++;
		if (count == 4)
			break;
		stairsLoop:
		i++;
	}

	uint8_t ** result = new uint8_t*[2];
	result[0] = map;
	result[1] = data;
	return result;
}

//	public static void main(String[] args) {
//		int d = 0;
//		while (true) {
//			int w = 128;
//			int h = 128;
//
//			uint8_t[] map = LevelGen.createAndValidateTopMap(w, h)[0];
//			// uint8_t[] map = LevelGen.createAndValidateUndergroundMap(w, h, (d++ % 3) + 1)[0];
//			// uint8_t[] map = LevelGen.createAndValidateSkyMap(w, h)[0];
//
//			BufferedImage img = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
//			int[] pixels = new int[w * h];
//			for (int y = 0; y < h; y++) {
//				for (int x = 0; x < w; x++) {
//					int i = x + y * w;
//
//					if (map[i] == Tile::water->id) pixels[i] = 0x000080;
//					if (map[i] == Tile::grass->id) pixels[i] = 0x208020;
//					if (map[i] == Tile::rock->id) pixels[i] = 0xa0a0a0;
//					if (map[i] == Tile::dirt->id) pixels[i] = 0x604040;
//					if (map[i] == Tile::sand->id) pixels[i] = 0xa0a040;
//					if (map[i] == Tile::tree->id) pixels[i] = 0x003000;
//					if (map[i] == Tile::lava->id) pixels[i] = 0xff2020;
//					if (map[i] == Tile::cloud->id) pixels[i] = 0xa0a0a0;
//					if (map[i] == Tile::stairsDown->id) pixels[i] = 0xffffff;
//					if (map[i] == Tile::stairsUp->id) pixels[i] = 0xffffff;
//					if (map[i] == Tile::cloudCactus->id) pixels[i] = 0xff00ff;
//				}
//			}
//			img.setRGB(0, 0, w, h, pixels, 0, w);
//			JOptionPane.showMessageDialog(null, null, "Another", JOptionPane.YES_NO_OPTION, new ImageIcon(img.getScaledInstance(w * 4, h * 4, Image.SCALE_AREA_AVERAGING)));
//		}
//	}
