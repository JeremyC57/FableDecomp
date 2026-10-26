// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `CameraResetToViewBehindHeroCount` -> `CreateCreatureDelayFrames` (+0x4c).
#include "engine/CGameScriptInterface.h"
#include <cstdio>


void __fastcall CGameScriptInterface_ResetCreatureCreationDelayFrames(CGameScriptInterface* self)
{
    self->CreateCreatureDelayFrames = 0;
}

int main()
{
    CGameScriptInterface obj;
    obj.CreateCreatureDelayFrames = 12345;
    CGameScriptInterface_ResetCreatureCreationDelayFrames(&obj);
    if (obj.CreateCreatureDelayFrames != 0) {
        std::printf("FAIL: expected 0 got %d\n", obj.CreateCreatureDelayFrames);
        return 1;
    }
    std::printf("CGameScriptInterface_006e7d40_TEST PASS\n");
    return 0;
}