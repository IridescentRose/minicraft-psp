#pragma once

#include <oslib/oslib.h>

class SpriteSheet {
public:
    SpriteSheet(OSL_IMAGE* image);
    virtual ~SpriteSheet();
    int width, height;
    int* pixels;
};