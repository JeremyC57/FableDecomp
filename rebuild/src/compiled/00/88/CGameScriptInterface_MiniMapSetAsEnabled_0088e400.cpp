// Leaf: ?MiniMapSetAsEnabled@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: void SetMiniMapEnabled(bool b); };

void CGameScriptInterface::MiniMapSetAsEnabled(bool enabled) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->SetMiniMapEnabled(enabled);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_MiniMapSetAsEnabled -> CGameScriptInterface::MiniMapSetAsEnabled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
