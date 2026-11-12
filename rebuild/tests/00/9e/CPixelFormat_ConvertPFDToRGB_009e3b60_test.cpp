// Behaviour test for CPixelFormat::ConvertPFDToRGB (0x009E3B60): each channel of a packed pixel is masked out at its position
// (blue lowest, then green, red, alpha) and scaled up to 8 bits.
#include <cstdio>
struct FormatInfo { long D3DFormat, PFType, BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
static const FormatInfo g_formats[3] = { {0,2,32,8,8,8,8,0,false}, {0,2,16,0,5,6,5,0,false}, {0,2,16,4,4,4,4,0,false} };
struct RGB { unsigned char B, G, R, A; };
struct PixelFormat {
    long FormatIndex;
    const FormatInfo& F() const { return g_formats[FormatIndex]; }
    unsigned long GPos() const { return F().BBits; }
    unsigned long RPos() const { return F().BBits + F().GBits; }
    unsigned long APos() const { return F().BBits + F().GBits + F().RBits; }
    static unsigned char Channel(unsigned long value, unsigned long bits, unsigned long pos)
    { return (unsigned char)(((value & (((1UL << bits) - 1) << pos)) >> pos) << (8 - bits)); }
    void ConvertPFDToRGB(const long& pfd, RGB& rgb) const
    {
        rgb.R = Channel(pfd, F().RBits, RPos());
        rgb.G = Channel(pfd, F().GBits, GPos());
        rgb.B = Channel(pfd, F().BBits, 0);
        rgb.A = Channel(pfd, F().AlphaBits, APos());
    }
};
int main()
{
    PixelFormat f; RGB c; bool ok = true;
    f.FormatIndex = 0; long argb = 0x80C04020; f.ConvertPFDToRGB(argb, c);
    ok = ok && c.A == 0x80 && c.R == 0xC0 && c.G == 0x40 && c.B == 0x20;
    f.FormatIndex = 1; long r565 = 0xFFE0 | 0x0011; f.ConvertPFDToRGB(r565, c);        // R = 31, G = 63, B = 17
    ok = ok && c.R == 0xF8 && c.G == 0xFC && c.B == 0x88 && c.A == 0x00;
    f.FormatIndex = 2; long a4 = 0xA5C3; f.ConvertPFDToRGB(a4, c);
    ok = ok && c.A == 0xA0 && c.R == 0x50 && c.G == 0xC0 && c.B == 0x30;
    if (!ok) { std::printf("009e3b60_TEST FAIL\n"); return 1; }
    std::printf("009e3b60_TEST PASS\n");
    return 0;
}