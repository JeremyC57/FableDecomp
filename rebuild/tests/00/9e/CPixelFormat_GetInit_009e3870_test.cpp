// Behaviour test for CPixelFormat::GetInit (0x009E3870): Type from PFType (2->1, 4->2, 5->3, 6->4, 7->5, others unchanged),
// the bit counts and the linear flag copied from the CD3DFormatInfo entry of FormatIndex.
#include <cstdio>
struct CD3DFormatInfo { int D3DFormat; int PFType; int BitDepth, AlphaBits, RBits, GBits, BBits, StencilBits; bool Linear; };
static const CD3DFormatInfo g_D3DFormatInfo[] = {
    { 21, 2, 32, 8, 8, 8, 8, 0, false },
    { 71, 3, 16, 0, 0, 0, 0, 0, true },
    { 50, 4, 8, 0, 8, 0, 0, 0, false },
    { 60, 5, 16, 0, 8, 8, 0, 0, true },
    { 70, 6, 16, 1, 5, 5, 5, 0, false },
    { 80, 7, 4, 4, 0, 0, 0, 0, true },
    { 90, 9, 24, 0, 8, 8, 8, 8, false },
};
class CPixelFormatInit {
public:
    unsigned char Type : 4;
    unsigned char Linear : 1;
    unsigned char ColourDepth, RBits, GBits, BBits, ABits;
};
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormatInit GetInit(unsigned char seedType) const
    {
        CPixelFormatInit init;
        init.Type = seedType;                            // stands in for the uninitialised local of retail
        switch (g_D3DFormatInfo[FormatIndex].PFType) {
        case 2: init.Type = 1; break;
        case 5: init.Type = 3; break;
        case 4: init.Type = 2; break;
        case 7: init.Type = 5; break;
        case 6: init.Type = 4; break;
        }
        init.ColourDepth = (unsigned char)g_D3DFormatInfo[FormatIndex].BitDepth;
        init.RBits = (unsigned char)g_D3DFormatInfo[FormatIndex].RBits;
        init.GBits = (unsigned char)g_D3DFormatInfo[FormatIndex].GBits;
        init.BBits = (unsigned char)g_D3DFormatInfo[FormatIndex].BBits;
        init.ABits = (unsigned char)g_D3DFormatInfo[FormatIndex].AlphaBits;
        init.Linear = g_D3DFormatInfo[FormatIndex].Linear;
        return init;
    }
};
int main()
{
    int wantType[7] = { 1, 9, 2, 3, 4, 5, 9 };
    for (int i = 0; i < 7; ++i) {
        CPixelFormat pf;
        pf.FormatIndex = i;
        CPixelFormatInit init = pf.GetInit(9);
        const CD3DFormatInfo& e = g_D3DFormatInfo[i];
        if (init.Type != wantType[i] || init.Linear != (e.Linear ? 1 : 0) || init.ColourDepth != e.BitDepth || init.RBits != e.RBits ||
            init.GBits != e.GBits || init.BBits != e.BBits || init.ABits != e.AlphaBits) { std::printf("009e3870_TEST FAIL\n"); return 1; }
    }
    std::printf("009e3870_TEST PASS\n");
    return 0;
}