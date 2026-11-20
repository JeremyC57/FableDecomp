#include <stdio.h>
#include <string.h>
// Behaviour test for 0x009BE420 CDisplayManager::ClearRenderTarget (links the row object).
// NF136 (2026-11-20, q4290): rewritten on the shared headers the row now uses (engine/CDisplayManager.h, engine/CPixelFormat.h,
//   engine/CD3DFormatInfo.h); the checks and the PassPattern are unchanged. The manager is a zeroed block with D3DDevice (+0x58) and
//   DepthBufferFormat (+0x1AC) written in place; the device is a fake object whose slot 43 (+0xAC) is the Clear fixture.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_SIGNED
#define FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#include "engine/CRGBColour.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"

extern const CD3DFormatInfo GD3DFormats[2] = {
    {(_D3DFORMAT)75, PF_TYPE_DEPTH, 32, 0, 0, 0, 0, 0, false},
    {(_D3DFORMAT)77, PF_TYPE_DEPTH, 32, 0, 0, 0, 0, 1, false},
};

static long g_observedFormat = 0;

long __fastcall CPixelFormat::FindFormatInfoIndexFromD3DFormat(_D3DFORMAT format)
{
    g_observedFormat = (long)format;
    return 1;
}

struct FakeDevice { void** vtable; };

static int g_clearCalls = 0;
static unsigned long g_observedFlags = 0;
static unsigned long g_observedColour = 0;
static float g_observedDepth = 0.0f;

static long __stdcall ClearFixture(FakeDevice*, unsigned long rectangleCount, const void* rectangles, unsigned long clearFlags,
                                   unsigned long colour, float depth, unsigned long stencil)
{
    ++g_clearCalls;
    if (rectangleCount != 0 || rectangles != 0 || stencil != 0)
        return -1;
    g_observedFlags = clearFlags;
    g_observedColour = colour;
    g_observedDepth = depth;
    return 0;
}

static double g_storage[(sizeof(CDisplayManager) + 7) / 8];

static int VerifyFlags(CDisplayManager& display, const CRGBColour& colour, unsigned long requested, unsigned long expected)
{
    display.ClearRenderTarget(colour, requested);
    return g_observedFlags == expected ? 0 : 1;
}

int main()
{
    void* vtable[0x2C];
    memset(vtable, 0, sizeof vtable);
    vtable[0x2B] = (void*)&ClearFixture;
    FakeDevice device = {vtable};
    memset(g_storage, 0, sizeof g_storage);
    unsigned char* raw = (unsigned char*)g_storage;
    FakeDevice* devicePointer = &device;
    long depthFormatIndex = 0;
    memcpy(raw + 0x58, &devicePointer, 4);
    memcpy(raw + 0x1AC, &depthFormatIndex, 4);
    CDisplayManager& display = *(CDisplayManager*)raw;

    CRGBColour colour;
    colour.IntValue = 0xA1B2C3D4;

    if (VerifyFlags(display, colour, 1, 2) != 0 ||
        VerifyFlags(display, colour, 2, 4) != 0 ||
        VerifyFlags(display, colour, 4, 1) != 0 ||
        VerifyFlags(display, colour, 7, 7) != 0)
    {
        printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (g_clearCalls != 4 ||
        g_observedFormat != 75 ||
        g_observedColour != colour.IntValue ||
        g_observedDepth != 1.0f)
    {
        printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR PASS\n");
    return 0;
}
