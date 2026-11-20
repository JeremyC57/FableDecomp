#include <stdio.h>
#include <string.h>
// Behaviour test for 0x009BEDC0 CDisplayManager::GetRenderTargetDimensions (links the row object).
// NF139 (2026-11-20, q4291): rewritten on the shared headers the row now uses (engine/CDisplayManager.h, engine/C2DExtentsI.h);
//   the checks and the PassPattern are unchanged. The manager is a zeroed block with RenderTargetDimensions (+0x194 X, +0x198 Y)
//   written in place.
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/CDisplayManager.h"

static double g_storage[(sizeof(CDisplayManager) + 7) / 8];

int main()
{
    memset(g_storage, 0, sizeof g_storage);
    unsigned char* raw = (unsigned char*)g_storage;
    long width = 1280;
    long height = 720;
    memcpy(raw + 0x194, &width, 4);
    memcpy(raw + 0x198, &height, 4);
    CDisplayManager& displayManager = *(CDisplayManager*)raw;

    const C2DExtentsI dimensions = displayManager.GetRenderTargetDimensions();
    if (dimensions.X != 1280 || dimensions.Y != 720)
    {
        return 1;
    }

    printf("FABLETLC_RENDER_TARGET_DIMENSIONS_BEHAVIOR PASS\n");
    return 0;
}
