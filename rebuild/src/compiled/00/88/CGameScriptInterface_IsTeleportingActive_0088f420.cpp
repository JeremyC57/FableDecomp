// Leaf: ?IsTeleportingActive@CGameScriptInterface@@UBE_NXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: char pad[0xd5]; bool active; };

bool CGameScriptInterface::IsTeleportingActive() const
{
    const CGameScriptInterface* self = this;
    return self->World->active;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_IsTeleportingActive -> CGameScriptInterface::IsTeleportingActive, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
