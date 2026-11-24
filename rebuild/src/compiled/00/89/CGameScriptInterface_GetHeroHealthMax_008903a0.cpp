// Leaf: ?GetHeroHealthMax@CGameScriptInterface@@UBEMXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern float g_defaultHealthMax;


float CGameScriptInterface::GetHeroHealthMax() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    CPlayer* t = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    CThingPlayerCreature* h = t->GetPControlledCreature();
    if (h != 0) {
        if ((*(unsigned char*)((char*)h + 0x91) & 1) == 0) {
            return *(float*)((char*)h + 0xb0);
        }
    }
    return g_defaultHealthMax;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetHeroHealthMax -> CGameScriptInterface::GetHeroHealthMax, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
