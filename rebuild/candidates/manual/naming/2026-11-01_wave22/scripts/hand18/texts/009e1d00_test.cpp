#include <math.h>
#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

class C2DExtentsI
{ public:
    fable_i32 X;
    fable_i32 Y;
};

class CDisplayManager
{ public:
    const C2DExtentsI GetRenderTargetDimensions();
};

class CSystemManager
{ public:
    fable_u8 unknown000[0x60];
    CDisplayManager* PDisplayManager;
};

double g_FableVirtualWidth_013961E8 = 1280.0;
static C2DExtentsI g_Dimensions = {640, 480};
static CDisplayManager g_DisplayManager;
static CSystemManager g_SystemManager;

const C2DExtentsI
CDisplayManager::GetRenderTargetDimensions()
{
    return g_Dimensions;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    return &g_SystemManager;
}

float FABLE_FASTCALL GFRoundVXToNearestPixel(double x);

int main()
{
    memset(&g_SystemManager, 0, sizeof(g_SystemManager));
    g_SystemManager.PDisplayManager = &g_DisplayManager;

    if (
        GFRoundVXToNearestPixel(17.9) != 16.0f ||
        GFRoundVXToNearestPixel(-0.1) != -2.0f ||
        GFRoundVXToNearestPixel(1280.0) != 1280.0f)
    {
        return 1;
    }

    printf("FABLETLC_ROUND_VX_TO_PIXEL_BEHAVIOR PASS\n");
    return 0;
}
