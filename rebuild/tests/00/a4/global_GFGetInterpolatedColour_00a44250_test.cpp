#include <cstdio>
// behaviour test of GFGetInterpolatedColour (0x00A44250): per channel (a * (256 - weight) + b * weight) >> 8, saturated to a byte
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
#ifdef _MSC_VER
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long weight)
{
    CRGBColour blend; unsigned long inverse; unsigned long zero;
    zero = 0;
    inverse = 256 - weight;
    __asm {
        movd      mm0, a
        movd      mm1, b
        movd      mm2, inverse
        movd      mm3, weight
        punpcklbw mm0, zero
        punpcklbw mm1, zero
        punpcklwd mm2, mm2
        punpcklwd mm3, mm3
        punpckldq mm2, mm2
        punpckldq mm3, mm3
        pmullw    mm0, mm2
        pmullw    mm1, mm3
        paddw     mm0, mm1
        psrlw     mm0, 8
        packuswb  mm0, mm0
        movd      blend, mm0
        emms
    }
    return CRGBColour(blend.R, blend.G, blend.B, blend.A);
}
#else
static unsigned char Mix(unsigned char x, unsigned char y, unsigned long weight)
{
    unsigned long v = ((unsigned long)x * ((256 - weight) & 0xffff) + (unsigned long)y * (weight & 0xffff)) & 0xffff;   // 16-bit lanes
    v >>= 8;
    return v > 255 ? 255 : (unsigned char)v;
}
CRGBColour GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long weight)
{
    return CRGBColour(Mix(a.R, b.R, weight), Mix(a.G, b.G, weight), Mix(a.B, b.B, weight), Mix(a.A, b.A, weight));
}
#endif
static bool Is(const CRGBColour& c, int r, int g, int b, int a) { return c.R == r && c.G == g && c.B == b && c.A == a; }
int main()
{
    CRGBColour a(200, 100, 0, 255), b(0, 20, 60, 255);
    bool ok = Is(GFGetInterpolatedColour(a, b, 0), 200, 100, 0, 255)       // all of a
           && Is(GFGetInterpolatedColour(a, b, 256), 0, 20, 60, 255)       // all of b
           && Is(GFGetInterpolatedColour(a, b, 128), 100, 60, 30, 255)     // the midpoint
           && Is(GFGetInterpolatedColour(a, b, 64), 150, 80, 15, 255);
    std::printf(ok ? "00a44250_TEST PASS\n" : "00a44250_TEST FAIL\n");
    return ok ? 0 : 1;
}