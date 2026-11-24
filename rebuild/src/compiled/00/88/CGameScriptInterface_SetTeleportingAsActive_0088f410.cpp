// Leaf: ?SetTeleportingAsActive@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char pad[0xd5]; unsigned char teleporting; };
void CGameScriptInterface::SetTeleportingAsActive(bool active) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->teleporting = active;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetTeleportingAsActive -> CGameScriptInterface::SetTeleportingAsActive, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
