#ifndef LEVELGEN_H_
#define LEVELGEN_H_

#include "../Random.h"
#include "tile/Tile.h"

typedef unsigned char uint8_t;

class LevelGen {
public:
	LevelGen(int w, int h, int featureSize);
	virtual ~LevelGen();
	float * values;
	static uint8_t ** createAndValidateTopMap(int w, int h);
	static uint8_t ** createAndValidateUndergroundMap(int w, int h, int depth);
	static uint8_t ** createAndValidateSkyMap(int w, int h);
	static Random * random;

private:
	int w, h;
	int widthMinusOne, heightMinusOne;


	inline float sample(int x, int y);
	inline void setSample(int x, int y, float sample);
	static uint8_t ** createTopMap(int w, int h);
	static uint8_t ** createUndergroundMap(int w, int h, int depth);
	static uint8_t ** createSkyMap(int w, int h);
};

#endif /* LEVELGEN_H_ */
