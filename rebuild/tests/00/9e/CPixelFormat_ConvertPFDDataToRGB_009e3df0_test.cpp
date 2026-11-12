// Behaviour test for CPixelFormat::ConvertPFDDataToRGB (0x009E3DF0): BitDepth / 8 raw bytes are copied into a zeroed word, each
// channel is masked out at its position (blue lowest, then green, red, alpha) and scaled up to 8 bits; no alpha bits -> alpha 0xFF.
#include <cstdio>
#include <cstring>
struct FormatInfo { long D3DFormat, PFType, BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
// Entry 4 is synthetic (not a D3D format): 16-bit depth but 24 mask bits, so red sits in byte 2 -- past BitDepth / 8 -- and only the
// copy length decides whether it reads the 0xEE trailer.
static const FormatInfo g_formats[5] = { {0,2,32,8,8,8,8,0,false}, {0,2,16,0,5,6,5,0,false}, {0,2,16,4,4,4,4,0,false}, {0,2,24,0,8,8,8,0,false},
                                         {0,2,16,0,8,8,8,0,false} };
struct RGB { unsigned char B, G, R, A; };
struct PixelFormat {
    long FormatIndex;
    const FormatInfo& F() const { return g_formats[FormatIndex]; }
    unsigned long GPos() const { return F().BBits; }
    unsigned long RPos() const { return F().BBits + F().GBits; }
    unsigned long APos() const { return F().BBits + F().GBits + F().RBits; }
    static unsigned char Channel(unsigned long value, unsigned long bits, unsigned long pos)
    { return (unsigned char)(((value & (((1UL << bits) - 1) << pos)) >> pos) << (8 - bits)); }
    void ConvertPFDDataToRGB(const unsigned char* data, RGB& colour) const
    {
        unsigned long pixel = 0;
        std::memcpy(&pixel, data, F().BitDepth / 8);
        colour.R = Channel(pixel, F().RBits, RPos());
        colour.G = Channel(pixel, F().GBits, GPos());
        colour.B = Channel(pixel, F().BBits, 0);
        if (F().AlphaBits)
            colour.A = Channel(pixel, F().AlphaBits, APos());
        else
            colour.A = 0xff;
    }
};
int main()
{
    PixelFormat f; RGB c; bool ok = true;
    // 0xEE trailers: entry 4's red mask covers byte 2, so it stays 0 only if exactly BitDepth / 8 = 2 bytes are copied.
    static const unsigned char argb[8]   = { 0x20, 0x40, 0xC0, 0x80, 0xEE, 0xEE, 0xEE, 0xEE };
    static const unsigned char r565[8]   = { 0xF1, 0xFF, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE };   // 0xFFF1: R = 31, G = 63, B = 17
    static const unsigned char a4[8]     = { 0xC3, 0xA5, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE };   // 0xA5C3
    static const unsigned char rgb888[8] = { 0x11, 0x22, 0x33, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE };
    f.FormatIndex = 0; c.A = c.R = c.G = c.B = 0x55; f.ConvertPFDDataToRGB(argb, c);
    ok = ok && c.A == 0x80 && c.R == 0xC0 && c.G == 0x40 && c.B == 0x20;
    f.FormatIndex = 1; c.A = c.R = c.G = c.B = 0x55; f.ConvertPFDDataToRGB(r565, c);
    ok = ok && c.R == 0xF8 && c.G == 0xFC && c.B == 0x88 && c.A == 0xFF;
    f.FormatIndex = 2; c.A = c.R = c.G = c.B = 0x55; f.ConvertPFDDataToRGB(a4, c);
    ok = ok && c.A == 0xA0 && c.R == 0x50 && c.G == 0xC0 && c.B == 0x30;
    f.FormatIndex = 3; c.A = c.R = c.G = c.B = 0x55; f.ConvertPFDDataToRGB(rgb888, c);
    ok = ok && c.R == 0x33 && c.G == 0x22 && c.B == 0x11 && c.A == 0xFF;
    static const unsigned char short16[8] = { 0x11, 0x22, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE };
    f.FormatIndex = 4; c.A = c.R = c.G = c.B = 0x55; f.ConvertPFDDataToRGB(short16, c);
    ok = ok && c.R == 0x00 && c.G == 0x22 && c.B == 0x11 && c.A == 0xFF;
    if (!ok) { std::printf("009e3df0_TEST FAIL\n"); return 1; }
    std::printf("009e3df0_TEST PASS\n");
    return 0;
}