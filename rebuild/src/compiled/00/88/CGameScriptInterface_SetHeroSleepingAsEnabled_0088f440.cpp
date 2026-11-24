// Leaf: ?SetHeroSleepingAsEnabled@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char _pad[0xd9]; unsigned char heroSleeping; };

void CGameScriptInterface::SetHeroSleepingAsEnabled(bool enabled) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->heroSleeping = enabled;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetHeroSleepingAsEnabled -> CGameScriptInterface::SetHeroSleepingAsEnabled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
