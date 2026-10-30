
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct HeroObj {
    unsigned char pad[0x91];
    unsigned char flags; // +0x91
    void GiveAbility(int a, int b, int c, int d);
};
class CThingPlayerCreature : public HeroObj { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

void __fastcall CGameScriptInterface_GiveHeroAbility(CGameScriptInterface* self, void* edx, int abilityId, int give)
{
    CPlayer* mgr = (&((CPlayerManager*)(*(void**)((char*)self + 0x14)))->GetMainPlayer());
    HeroObj* h = mgr->GetPControlledCreature();
    if (h != 0 && (h->flags & 1) == 0) {
        h->GiveAbility(abilityId, give, -1, 1);
    }
}