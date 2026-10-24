// Behaviour test for CPixelFormat::Initialise(_D3DFORMAT) (0x009E3830): finds the row of a format in the
// BitDepth(-1)-terminated CD3DFormatInfo table, -1 when absent (including formats listed only after the
// terminator).
#include <stdio.h>

enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_R8G8B8 = 20, D3DFMT_A8R8G8B8 = 21, D3DFMT_X8R8G8B8 = 22,
                  D3DFMT_R5G6B5 = 23, D3DFMT_D16 = 80, D3DFMT_FORCE_DWORD = 0x7fffffff };

struct CD3DFormatInfo {
    _D3DFORMAT D3DFormat;
    long PFType;
    long BitDepth;
    long AlphaBits;
    long RBits;
    long GBits;
    long BBits;
    long StencilBits;
    bool Linear;
};

const CD3DFormatInfo g_D3DFormatInfo[] = {
    { D3DFMT_R8G8B8,   2, 24, 0, 8, 8, 8, 0, false },
    { D3DFMT_A8R8G8B8, 2, 32, 8, 8, 8, 8, 0, false },
    { D3DFMT_X8R8G8B8, 2, 32, 0, 8, 8, 8, 0, false },
    { D3DFMT_D16,      3, 16, 0, 0, 0, 0, 0, false },
    { D3DFMT_UNKNOWN,  0, -1, 0, 0, 0, 0, 0, false },
    { D3DFMT_R5G6B5,   2, 16, 0, 5, 6, 5, 0, false },   // past the terminator: never found
};

class CPixelFormat
{
public:
    long FormatIndex;
    void Initialise(_D3DFORMAT format);
};

static inline long FindFormatIndex(_D3DFORMAT format)
{
    long index = 0;
    do
    {
        if (g_D3DFormatInfo[index].D3DFormat == format)
            return index;
        ++index;
    } while (g_D3DFormatInfo[index].BitDepth != -1);
    return -1;
}

void CPixelFormat::Initialise(_D3DFORMAT format)
{
    FormatIndex = FindFormatIndex(format);
}

int main()
{
    CPixelFormat f;
    f.FormatIndex = 99; f.Initialise(D3DFMT_R8G8B8);   if (f.FormatIndex != 0) { printf("FAIL first\n"); return 1; }
    f.FormatIndex = 99; f.Initialise(D3DFMT_X8R8G8B8); if (f.FormatIndex != 2) { printf("FAIL middle\n"); return 1; }
    f.FormatIndex = 99; f.Initialise(D3DFMT_D16);      if (f.FormatIndex != 3) { printf("FAIL last\n"); return 1; }
    f.FormatIndex = 99; f.Initialise(D3DFMT_R5G6B5);   if (f.FormatIndex != -1) { printf("FAIL past end\n"); return 1; }
    f.FormatIndex = 99; f.Initialise((_D3DFORMAT)12345); if (f.FormatIndex != -1) { printf("FAIL absent\n"); return 1; }
    printf("FABLETLC_PIXEL_FORMAT_INITIALISE PASS\n");
    return 0;
}
