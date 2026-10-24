#include <cstdio>
// Behaviour test for CPixelFormat::CPixelFormat(long, long, long, long) (retail 0x0041C850): the constructor hands
// the four channel widths and `false` to Initialise, which looks the format up in the table.
struct SFormatEntry { long Type, ColourDepth, ABits, RBits, GBits, BBits; };
static const SFormatEntry g_formats[] = {
    { 2, 24, 0, 8, 8, 8 }, { 2, 32, 8, 8, 8, 8 }, { 2, 16, 0, 5, 6, 5 }, { -1, 0, 0, 0, 0, 0 }
};
static int g_lastOption = -1;
struct CPixelFormat {
    long FormatIndex;
    void Initialise(long r, long g, long b, long a, bool option)
    {
        g_lastOption = option ? 1 : 0;
        for (long i = 0; g_formats[i].Type != -1; ++i)
            if (g_formats[i].Type == 2 && g_formats[i].RBits == r && g_formats[i].GBits == g && g_formats[i].BBits == b && g_formats[i].ABits == a)
            { FormatIndex = i; return; }
    }
    CPixelFormat(long rBits, long gBits, long bBits, long aBits) : FormatIndex(-7) { Initialise(rBits, gBits, bBits, aBits, false); }
};
int main()
{
    CPixelFormat a(8, 8, 8, 8), b(5, 6, 5, 0), c(8, 8, 8, 0), d(4, 4, 4, 4);
    int ok = a.FormatIndex == 1 && b.FormatIndex == 2 && c.FormatIndex == 0 && d.FormatIndex == -7 && g_lastOption == 0;
    if (!ok) { std::printf("0041c850_TEST FAIL\n"); return 1; }
    std::printf("0041c850_TEST PASS\n");
    return 0;
}