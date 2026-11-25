// Leaf: ?GetHeroHealthPercentage@CGameScriptInterface@@UBEMXZ
// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_defaultHealthPct (table ?g_defaultHealthPct@@3MA -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
class CThingPhysical { public: float GetHealthPercentage() const; };   // 0x006A4E40 (Ego_r.pdb CThingPhysical, const)

float CGameScriptInterface::GetHeroHealthPercentage() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    void* mgr = self->PlayerManager;
    CThingPlayerCreature* hero = (&((CPlayerManager*)mgr)->GetMainPlayer())->GetPControlledCreature();
    if (hero != 0 && (*(unsigned char*)((char*)hero + 0x91) & 0x1) == 0) {
        float v = ((CThingPhysical*)hero)->GetHealthPercentage();
        return v;
    }
    return 0.0f;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetHeroHealthPercentage -> CGameScriptInterface::GetHeroHealthPercentage, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
