#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern float __fastcall Sub_GetHealthPct(CThingPlayerCreature* hero);
extern float g_defaultHealthPct;

float __fastcall CGameScriptInterface_GetHeroHealthPercentage(CGameScriptInterface* self)
{
    void* mgr = self->PlayerManager;
    CThingPlayerCreature* hero = (&((CPlayerManager*)mgr)->GetMainPlayer())->GetPControlledCreature();
    if (hero != 0 && (*(unsigned char*)((char*)hero + 0x91) & 0x1) == 0) {
        float v = Sub_GetHealthPct(hero);
        return v;
    }
    return g_defaultHealthPct;
}