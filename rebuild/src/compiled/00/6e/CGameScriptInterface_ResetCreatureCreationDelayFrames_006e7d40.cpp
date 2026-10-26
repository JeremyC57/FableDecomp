
// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `CameraResetToViewBehindHeroCount` -> `CreateCreatureDelayFrames` (+0x4c).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CGameScriptInterface_ResetCreatureCreationDelayFrames(CGameScriptInterface* self)
{
    self->CreateCreatureDelayFrames = 0;
}