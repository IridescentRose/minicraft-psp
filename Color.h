#pragma once

class Color {
public:
static int get(int a, int b, int c, int d)
{
int i = (get(d) << 24) + (get(c) << 16) + (get(b) << 8) + (get(a));
return i;
}
static int get(int d)
{
    if (d < 0) return 255;
    int r = d / 100 % 10;
    int g = d / 10 % 10;
    int b = d % 10;
    return r * 36 + g * 6 + b;
}

};