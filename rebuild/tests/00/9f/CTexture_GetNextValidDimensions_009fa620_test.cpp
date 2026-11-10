// Behaviour test for CTexture::GetNextValidDimensions (0x009FA620): host model of the size rounding (each side to the next power of
// two when the device needs power-of-two textures, 0 stays 0) with the top-bit search of GFGetTopBit.
#include <stdio.h>

static unsigned long TopBit(unsigned long value)
{
    if (value == 0)
        return (unsigned long)-1;
    if (value >= 0x80000000UL)
        return 31;
    ++value;
    long r = ((long)((1L << 16) - (long)value) >> 31) & 16;
    r += ((long)((1L << (r + 8)) - (long)value) >> 31) & 8;
    r += ((long)((1L << (r + 4)) - (long)value) >> 31) & 4;
    r += ((long)((1L << (r + 2)) - (long)value) >> 31) & 2;
    r += ((long)((1L << (r + 1)) - (long)value) >> 31) & 1;
    return (unsigned long)r;
}
static unsigned long NextPow2(unsigned long v) { if (v == 0) return 0; return 1UL << TopBit(v * 2 - 1); }
struct Ext { int X, Y; };
static Ext NextValid(const Ext& size, bool pow2)
{
    Ext r = size;
    if (pow2) { r.X = (int)NextPow2(r.X); r.Y = (int)NextPow2(r.Y); }
    return r;
}

int main()
{
    int fails = 0;
    Ext a = { 100, 0 };
    Ext r = NextValid(a, true);
    if (r.X != 128 || r.Y != 0) ++fails;
    Ext b = { 64, 1 };
    r = NextValid(b, true);
    if (r.X != 64 || r.Y != 1) ++fails;
    Ext c = { 513, 3 };
    r = NextValid(c, false);
    if (r.X != 513 || r.Y != 3) ++fails;
    r = NextValid(c, true);
    if (r.X != 1024 || r.Y != 4) ++fails;
    if (TopBit(0) != (unsigned long)-1 || TopBit(1) != 0 || TopBit(255) != 7 || TopBit(256) != 8 || TopBit(0x80000001UL) != 31) ++fails;
    if (fails == 0) printf("009fa620_TEST PASS\n"); else printf("009fa620_TEST FAIL %d\n", fails);
    return fails;
}