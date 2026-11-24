// Leaf: ?GetGuildSealRecallAngleXY@CGameScriptInterface@@UBEMXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: char pad[0x160]; float angle; };

float CGameScriptInterface::GetGuildSealRecallAngleXY() const
{
    const CGameScriptInterface* self = this;
    return self->World->angle;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetGuildSealRecallAngleXY -> CGameScriptInterface::GetGuildSealRecallAngleXY, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
