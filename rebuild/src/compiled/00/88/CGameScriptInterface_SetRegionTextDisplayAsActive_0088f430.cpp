// Leaf: ?SetRegionTextDisplayAsActive@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char _pad[0xdf]; unsigned char regionTextActive; };

void CGameScriptInterface::SetRegionTextDisplayAsActive(bool active) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->regionTextActive = (unsigned char)active;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetRegionTextDisplayAsActive -> CGameScriptInterface::SetRegionTextDisplayAsActive, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
