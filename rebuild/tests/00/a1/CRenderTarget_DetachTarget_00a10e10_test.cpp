#include <cstdio>
#include <cstring>
// Behaviour test for 0x00A10E10 CRenderTarget::DetachTarget (links the row object).
// NF142 (2026-11-20, q6305): rewritten on engine/CRenderTarget.h, the header the row now uses (the old test was a standalone copy over the
//   S / Sub stand-ins). RenderToSurface is +0x04 with its PD3DSurface at +0x08; CSurface::Uninitialise is the test's fixture. PassPattern
//   unchanged.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_GETD3DSURFACE
#include "engine/CRenderTarget.h"

static const void* g_arg = 0;
static int g_calls = 0;
void CSurface::Uninitialise() { g_arg = this; ++g_calls; }

int main()
{
    static double storage[(sizeof(CRenderTarget) + 7) / 8];
    char* raw = (char*)storage;
    std::memset(storage, 0, sizeof storage);
    CRenderTarget* target = (CRenderTarget*)raw;
    target->DetachTarget();
    if (g_calls != 0) { std::printf("00a10e10_TEST FAIL\n"); return 1; }
    void* surface = (void*)1;
    std::memcpy(raw + 8, &surface, 4);
    target->DetachTarget();
    if (g_calls != 1 || g_arg != raw + 4) { std::printf("00a10e10_TEST FAIL\n"); return 1; }
    std::printf("00a10e10_TEST PASS\n");
    return 0;
}
