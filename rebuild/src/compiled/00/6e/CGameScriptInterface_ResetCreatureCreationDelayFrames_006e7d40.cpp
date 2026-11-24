// Leaf: ?ResetCreatureCreationDelayFrames@CGameScriptInterface@@UAEXXZ

// Layout: retail STLport 4.6 header offsets; member names re-bound by the bytes this body touches: `CameraResetToViewBehindHeroCount` -> `CreateCreatureDelayFrames` (+0x4c).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
void CGameScriptInterface::ResetCreatureCreationDelayFrames()
{
    CGameScriptInterface* self = this;
    self->CreateCreatureDelayFrames = 0;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_ResetCreatureCreationDelayFrames -> CGameScriptInterface::ResetCreatureCreationDelayFrames, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
