// Behaviour test for 0x009C11A0 CDisplayManager::CCreateDeviceParameters copy constructor (NF140: on engine/CDisplayManager.h, the
// nested parameter block; the compiler-generated memberwise copy with C2DExtentsI's user copy constructor, as the row).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#include <cstdio>
#include <cstring>
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
typedef CDisplayManager::CCreateDeviceParameters Params;
int main()
{
    Params a;
    std::memset(&a, 0xCD, sizeof(a));
    a.WinHandle = reinterpret_cast<HWND>(0x1234);
    a.BackBufferDimensions.X = 1024; a.BackBufferDimensions.Y = 768;
    a.BackBufferBitDepth = 32; a.ZBufferBitDepth = 24; a.BackBufferCount = 2; a.RefreshRate = 60;
    a.ExclusiveMode = true; a.PresentationInterval.PresentImmediate = true; a.PresentationInterval.PresentImmediateIfSyncMissed = false;
    a.PresentationInterval.VSyncCount = 5;
    a.MultisampleType = SURFACE_MULTISAMPLE_4;
    Params b(a);
    if (b.WinHandle != a.WinHandle || b.BackBufferDimensions.X != 1024 || b.BackBufferDimensions.Y != 768
        || b.BackBufferBitDepth != 32 || b.ZBufferBitDepth != 24 || b.BackBufferCount != 2 || b.RefreshRate != 60
        || !b.ExclusiveMode || !b.PresentationInterval.PresentImmediate || b.PresentationInterval.PresentImmediateIfSyncMissed
        || b.PresentationInterval.VSyncCount != 5 || b.MultisampleType != SURFACE_MULTISAMPLE_4 || sizeof(Params) != 44)
    { std::printf("009c11a0_TEST FAIL\n"); return 1; }
    std::printf("009c11a0_TEST PASS\n");
    return 0;
}
