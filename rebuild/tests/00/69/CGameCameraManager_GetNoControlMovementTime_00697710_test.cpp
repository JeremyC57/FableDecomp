// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `InsideHeroCount` -> `ControlNoControlMovementStartFrame` (+0xc0).
#include "engine/CGameCameraManager.h"
#include <stdio.h>


extern int __cdecl GetEngineTimeMs(void);
extern float g_defaultNoControlMovementTime;
extern int   g_noControlTimeDivisor;

float __fastcall CGameCameraManager_GetNoControlMovementTime(CGameCameraManager *manager);

// ---- trivial extern stub defs so the test links ----
float g_defaultNoControlMovementTime = -1.0f;
int   g_noControlTimeDivisor = 1000;
static int g_fakeNow = 5500;
int __cdecl GetEngineTimeMs(void) { return g_fakeNow; }

int main(void)
{
    CGameCameraManager cm;
    for (int i = 0; i < (int)sizeof(cm); ++i) ((char*)&cm)[i] = 0;

    // Case 1: start time == 0 -> returns the default global (fld path).
    cm.ControlNoControlMovementStartFrame = 0;
    float r0 = CGameCameraManager_GetNoControlMovementTime(&cm);

    // Case 2: start time set -> (now - start)/divisor. now=5500 -> (5500-2500)/1000 = 3.0
    cm.ControlNoControlMovementStartFrame = 2500;
    float r1 = CGameCameraManager_GetNoControlMovementTime(&cm);

    int ok = (r0 == -1.0f) && (r1 == 3.0f);

    printf("r0=%f r1=%f\n", r0, r1);
    if (ok)
        printf("CGAMECAMERAMANAGER_00697710_TEST PASS\n");
    else
        printf("CGAMECAMERAMANAGER_00697710_TEST FAIL\n");
    return ok ? 0 : 1;
}
