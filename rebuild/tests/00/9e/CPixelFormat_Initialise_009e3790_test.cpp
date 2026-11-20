#include <cstdio>
#include <cstring>
// Behaviour test for 0x009E3790 CPixelFormat::Initialise(long, long, long, long, bool) (links the row object).
// NF139 (2026-11-20, q4295): rewritten on the shared headers the row now uses (engine/CPixelFormat.h retail branch,
//   engine/CD3DFormatInfo.h); the format table GD3DFormats is defined here with the same seven records as before, and the
//   protected FormatIndex is read and written through the object's bytes. Checks and PassPattern unchanged.
#define FABLE_RETAIL_CPIXELFORMAT
#include "engine/CPixelFormat.h"

extern const CD3DFormatInfo GD3DFormats[7] = {
    { (_D3DFORMAT)20, PF_TYPE_RGBA, 24, 0, 8, 8, 8, 0, false },
    { (_D3DFORMAT)21, PF_TYPE_RGBA, 32, 8, 8, 8, 8, 0, false },
    { (_D3DFORMAT)22, PF_TYPE_RGBA, 32, 0, 8, 8, 8, 0, false },
    { (_D3DFORMAT)23, PF_TYPE_RGBA, 16, 0, 5, 6, 5, 0, false },
    { (_D3DFORMAT)41, PF_TYPE_UNKNOWN, 16, 8, 5, 6, 5, 0, false },
    { (_D3DFORMAT)25, PF_TYPE_RGBA, 16, 1, 5, 5, 5, 0, false },
    { (_D3DFORMAT)102, PF_TYPE_UNKNOWN, -1, 0, 0, 0, 0, 0, false },
};

static long IndexOf(const CPixelFormat& p) { long v; std::memcpy(&v, &p, 4); return v; }

int main()
{
    CPixelFormat p; long start = -7; std::memcpy(&p, &start, 4);
    p.Initialise(8, 8, 8, 8, false); if (IndexOf(p) != 1) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    p.Initialise(8, 8, 8, 0, false); if (IndexOf(p) != 0) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    p.Initialise(5, 5, 5, 1, true); if (IndexOf(p) != 5) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    p.Initialise(5, 6, 5, 8, false); if (IndexOf(p) != 5) { std::printf("009e3790_TEST FAIL\n"); return 1; }   // only a non-RGB record matches: unchanged
    p.Initialise(1, 2, 3, 4, false); if (IndexOf(p) != 5) { std::printf("009e3790_TEST FAIL\n"); return 1; }
    std::printf("009e3790_TEST PASS\n");
    return 0;
}
