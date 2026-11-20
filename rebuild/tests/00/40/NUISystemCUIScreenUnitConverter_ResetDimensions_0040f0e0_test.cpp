#include <stdio.h>
#include <string.h>
// Behaviour test for 0x0040F0E0 NUISystem::CUIScreenUnitConverter::ResetDimensions (links the row object).
// NF142 (2026-11-20, q4291): rewritten on the shared headers the row now uses (the old test was a standalone copy over Device / Dims
//   stand-ins). The system manager is a zeroed block with PDisplayManager's Data word (+0x60) pointing at a zeroed display manager
//   block; GetRenderTargetDimensions is the test's fixture (640 x 480). The statics are read back through the header's inline
//   GetBackBufferWidth / GetBackBufferHeight (IsUsingRelativeCoords set). PassPattern unchanged.
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_RESETDIMENSIONS
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"

bool  NUISystem::CUIScreenUnitConverter::IsUsingRelativeCoords = true;
float NUISystem::CUIScreenUnitConverter::BackBufferWidth = 0.0f;
float NUISystem::CUIScreenUnitConverter::BackBufferHeight = 0.0f;
float NUISystem::CUIScreenUnitConverter::DefaultBackBufferWidth = 640.0f;
float NUISystem::CUIScreenUnitConverter::DefaultBackBufferHeight = 480.0f;

static double g_systemStorage[(sizeof(CSystemManager) + 7) / 8];
static double g_displayStorage[(sizeof(CDisplayManager) + 7) / 8];
static int g_dimensionCalls = 0;

CSystemManager* __fastcall GFGetSystemManager()
{
    return (CSystemManager*)g_systemStorage;
}

const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{
    ++g_dimensionCalls;
    C2DExtentsI dimensions;
    dimensions.X = ((void*)this == (void*)g_displayStorage) ? 640 : -1;
    dimensions.Y = 480;
    return dimensions;
}

int main(void)
{
    memset(g_systemStorage, 0, sizeof g_systemStorage);
    memset(g_displayStorage, 0, sizeof g_displayStorage);
    void* display = g_displayStorage;
    memcpy((unsigned char*)g_systemStorage + 0x60, &display, 4);

    NUISystem::CUIScreenUnitConverter::ResetDimensions();
    float w = NUISystem::CUIScreenUnitConverter::GetBackBufferWidth();
    float h = NUISystem::CUIScreenUnitConverter::GetBackBufferHeight();
    if (g_dimensionCalls == 1 && w == 640.0f && h == 480.0f) {
        printf("_global_0040f0e0_TEST PASS\n");
        return 0;
    }
    printf("FAIL calls=%d w=%f h=%f\n", g_dimensionCalls, w, h);
    return 1;
}
