#include <cstdio>
struct CD3DFormatInfo { long D3DFormat, PFType, BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
static const CD3DFormatInfo g_D3DFormatInfo[] = {
    { 20, 2, 24, 0, 8, 8, 8, 0, false },
    { 21, 2, 32, 8, 8, 8, 8, 0, false },
    { 22, 2, 32, 0, 8, 8, 8, 0, false },
    { 23, 2, 16, 0, 5, 6, 5, 0, false },
    { 41, 1, 16, 8, 5, 6, 5, 0, false },
    { 25, 2, 16, 1, 5, 5, 5, 0, false },
    { 102, 1, -1, 0, 0, 0, 0, 0, false },
};
struct CPixelFormat {
    long FormatIndex;
    void Initialise(long rBits, long gBits, long bBits, long aBits, bool)
    {
        long index = 0;
        const CD3DFormatInfo* info = g_D3DFormatInfo;
        do {
            if (info->PFType == 2 && info->RBits == rBits && info->GBits == gBits && info->BBits == bBits && info->AlphaBits == aBits) {
                FormatIndex = index;
                return;
            }
            ++info;
            ++index;
        } while (info->BitDepth != -1);
    }
};
int main()
{
    CPixelFormat p; p.FormatIndex = -7;
    p.Initialise(8, 8, 8, 8, false); if (p.FormatIndex != 1) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    p.Initialise(8, 8, 8, 0, false); if (p.FormatIndex != 0) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    p.Initialise(5, 5, 5, 1, true); if (p.FormatIndex != 5) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    p.Initialise(5, 6, 5, 8, false); if (p.FormatIndex != 5) { std::printf("009e3790_TEST FAIL\n"); return 1; }   // only a non-RGB record matches: unchanged
    p.Initialise(1, 2, 3, 4, false); if (p.FormatIndex != 5) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    std::printf("009e3790_TEST PASS\n");
    return 0;
}
