// Leaf: ?SetSavingAsEnabled@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char pad[0xd7]; unsigned char flagD7; };

void CGameScriptInterface::SetSavingAsEnabled(bool enabled) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->flagD7 = (unsigned char)enabled;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetSavingAsEnabled -> CGameScriptInterface::SetSavingAsEnabled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
