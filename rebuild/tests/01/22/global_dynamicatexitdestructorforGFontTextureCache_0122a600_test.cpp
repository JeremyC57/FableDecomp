#include <cstdio>
// Behaviour test for 0x0122A600, the dynamic atexit destructor of the font TU's static CTexture GFontTextureCache.
// NF142 (2026-11-20, q4296): rewritten on engine/CTexture.h, the header the row now uses (the old test modelled the AutoTinyThunkTarget
//   stand-in). The static is defined the row's way; at exit its compiler-emitted atexit destructor runs the inline ~CTexture, i.e.
//   Uninitialise on the static. A checker static defined before it is destroyed after it and reports. PassPattern unchanged.
#define FABLE_RETAIL_CTEXTURE
#include "fable_compiler.h"
#include "engine/CTexture.h"

static int g_uninitialised = 0;
static const void* g_last = 0;
static const void* g_expected = 0;

bool CTexture::Initialise(const C2DExtentsI&, long, const CPixelFormat&, unsigned long, ESurfacePool, bool, bool) { return false; }
void CTexture::Uninitialise() { g_last = this; ++g_uninitialised; }

struct CExitChecker {
    ~CExitChecker()
    {
        if (g_uninitialised == 1 && g_last == g_expected)
            std::printf("AUTO_TINY_0122a600_TEST PASS\n");
        else
            std::printf("AUTO_TINY_0122a600_TEST FAIL\n");
        std::fflush(stdout);
    }
};
static CExitChecker g_checker;          // constructed first, destroyed last

namespace {
CTexture GFontTextureCache;             // 0x013C5A80 in retail
}

int main()
{
    g_expected = &GFontTextureCache;
    return g_uninitialised == 0 ? 0 : 1;
}
