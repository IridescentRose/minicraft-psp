#include "SpriteSheet.h"
#include "Screen.h"
#include <oslib/oslib.h>

Screen::Screen(int w, int h, SpriteSheet* sheet) :
    xOffset(0), yOffset(0)
{
    this->sheet = sheet;
    this->w = w;
    this->h = h;
    dither = new int[16];
    dither[0] = 0;
    dither[1] = 8;
    dither[2] = 2;
    dither[3] = 10;
    dither[4] = 12;
    dither[5] = 4;
    dither[6] = 14;
    dither[7] = 6;
    dither[8] = 3;
    dither[9] = 11;
    dither[10] = 1;
    dither[11] = 9;
    dither[12] = 15;
    dither[13] = 7;
    dither[14] = 13;
    dither[15] = 5;
    pixels = new ushort[w * h];
}

Screen::~Screen()
{
}

void Screen::clear(int color)
{
    int length = w * h;
    for (int i = 0; i < length; ++i)
        pixels[i] = color;
}

void Screen::render(int xp, int yp, int tile, int colors, int bits)
{
    xp -= xOffset;
    yp -= yOffset;
    bool mirrorX = (bits & BIT_MIRROR_X) > 0;
    bool mirrorY = (bits & BIT_MIRROR_Y) > 0;

    int xTile = tile % 32;
    int yTile = tile / 32;
    int toffs = xTile * 8 + yTile * 8 * sheet->width;

    int* source = sheet->pixels;

    for (int y = 0; y < 8; y++)
    {
        int ys = y;
        if (mirrorY) ys = 7 - y;
        int yPlusYp = y + yp;
        if (yPlusYp < 0 || yPlusYp >= h) continue;
        int ysByWidthPlusToffs = ys * sheet->width + toffs;
        int yPlusYpByWidth = yPlusYp * w;
        for (int x = 0; x < 8; x++)
        {
            int xPlusXp = x + xp;
            if (xPlusXp < 0 || xPlusXp >= w) continue;
            int xs = x;
            if (mirrorX) xs = 7 - x;
            ushort col = (colors >> (source[xs + ysByWidthPlusToffs])) & 255;
            if (col < 255) pixels[xPlusXp + yPlusYpByWidth] = col;
        }
    }
}

void Screen::setOffset(int xOffset, int yOffset)
{
    this->xOffset = xOffset;
    this->yOffset = yOffset;
}

void Screen::overlay(Screen * screen, int xa, int ya)
{
    ushort * oPixels = screen->pixels;
    int i = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (oPixels[i] / 10 <= dither[((x + xa) & 3) + ((y + ya) & 3) * 4]) pixels[i] = 0;
            i++;
        }
    }
}

void Screen::renderLight(int x, int y, int r)
{
    x -= xOffset;
    y -= yOffset;
    int x0 = x - r;
    int x1 = x + r;
    int y0 = y - r;
    int y1 = y + r;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > w) x1 = w;
    if (y1 > h) y1 = h;
    for (int yy = y0; yy < y1; yy++)
    {
        int yd = yy - y;
        yd = yd * yd;
        int yyByWidth = yy * w;
        for (int xx = x0; xx < x1; xx++)
        {
            int xd = xx - x;
            int dist = xd * xd + yd;
            int r2 = r * r;
            if (dist <= r2)
            {
                int br = 255 - dist * 255 / r2;
                if (pixels[xx + yyByWidth] < br) pixels[xx + yyByWidth] = br;
            }
        }
    }
}

void Screen::render2(int xp, int yp, int tile, int colors, int bits)
{
    xp -= xOffset;
    yp -= yOffset;
    bool mirrorX = (bits & BIT_MIRROR_X) > 0;
    bool mirrorY = (bits & BIT_MIRROR_Y) > 0;

    int xTile = tile % 32;
    int yTile = tile / 32;
    int toffs = xTile * 8 + yTile * 8 * sheet->width;

    for (int y = 0; y < 8; y++)
    {
        int ys = y;
        if (mirrorY) ys = 7 - y;
        if (y + yp < 0 || y + yp >= h) continue;
        for (int x = 0; x < 8; x++)
        {
            if (x + xp < 0 || x + xp >= w) continue;
            int xs = x;
            if (mirrorX) xs = 7 - x;
            int col = (colors >> (sheet->pixels[xs + ys * sheet->width + toffs] * 8)) & 255;
            if (col < 255) pixels[(x + xp) + (y + yp) * w] = col;
        }
    }
    for (int y = 0; y < 8; y++)
    {
        int ys = y;
        if (mirrorY) ys = 7 - y;
        if (y + yp < 0 || y + yp >= h) continue;
        memcpy((int*)(pixels + xp + (y + yp) * w),
               (int*)(sheet->pixels + ys * sheet->width + toffs),
               16 * 2);
    }
}
