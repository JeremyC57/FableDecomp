// Leaf: ?GetCountdownTimer@CGameScriptInterface@@UBEMXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: char pad[0xa0]; int frames; };

float CGameScriptInterface::GetCountdownTimer() const
{
    const CGameScriptInterface* self = this;
    return (float)self->World->frames;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetCountdownTimer -> CGameScriptInterface::GetCountdownTimer, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
