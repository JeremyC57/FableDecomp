// NF418 (2026-11-25): the row now returns the float literal 0.0f (retail's pooled __real@00000000, 0x0122DEDC) instead of the data symbol
// g_defaultNoControlMovementTime; the test's definition of that symbol is gone and case 1 expects 0.0f (a mutated literal fails).
// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `InsideHeroCount` -> `ControlNoControlMovementStartFrame` (+0xc0).
#include "engine/CGameCameraManager.h"
#include "engine/CMainGameComponent.h"
#include <stdio.h>



float __fastcall CGameCameraManager_GetNoControlMovementTime(CGameCameraManager *manager);

// ---- trivial extern stub defs so the test links ----
// NF166 (round 10): the divisor 0x01375550 is the protected static CMainGameComponent::ConstantFPS (engine/CMainGameComponent.h);
//   the test defines it (was the stand-in g_noControlTimeDivisor).
long CMainGameComponent::ConstantFPS = 1000;
static int g_fakeNow = 5500;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
long __fastcall CWorld::GetFrame() { return g_fakeNow; }

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

    int ok = (r0 == 0.0f) && (r1 == 3.0f);

    printf("r0=%f r1=%f\n", r0, r1);
    if (ok)
        printf("CGAMECAMERAMANAGER_00697710_TEST PASS\n");
    else
        printf("CGAMECAMERAMANAGER_00697710_TEST FAIL\n");
    return ok ? 0 : 1;
}
