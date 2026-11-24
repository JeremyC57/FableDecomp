// Leaf: ?SetSummonerDeathExplosionAffectsHero@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char _pad[0xe1]; unsigned char flagE1; };

void CGameScriptInterface::SetSummonerDeathExplosionAffectsHero(bool doesAffect) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->flagE1 = doesAffect;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetSummonerDeathExplosionAffectsHero -> CGameScriptInterface::SetSummonerDeathExplosionAffectsHero, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
