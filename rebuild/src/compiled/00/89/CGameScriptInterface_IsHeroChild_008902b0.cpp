#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Hero {
    char pad[0x91];
    unsigned char flags;
};
class CThingPlayerCreature : public Hero { };

class CPlayerManager { public: int dummy; };

class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
extern "C" CPlayer* __fastcall GSI_GetWorld(CPlayerManager* w);
extern "C" bool   __fastcall Hero_CheckChild(Hero* h);

bool __fastcall CGameScriptInterface_IsHeroChild(CGameScriptInterface* self)
{
    Hero* h = GSI_GetWorld(self->PlayerManager)->GetPControlledCreature();
    if (h != 0 && (h->flags & 1) == 0)
        return Hero_CheckChild(h);
    return false;
}