#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(OSL_IMAGE* image) : width(image->realSizeX), height(image->realSizeY)
{
    pixels = new int[image->totalSize / sizeof(int)];
    int* imageData = reinterpret_cast<int*>(image->data);

    for (uint i = 0; i < image->totalSize / sizeof(int); i++)
    {
        pixels[i] = (imageData[i] & 0xFF) / 64 * 8;
    }
}

SpriteSheet::~SpriteSheet()
{
    delete[] pixels;
    pixels = nullptr;
}