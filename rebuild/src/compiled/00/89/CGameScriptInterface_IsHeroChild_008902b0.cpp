#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Hero {
    char pad[0x91];
    unsigned char flags;
};
class CThingPlayerCreature : public Hero { };

#include "engine/CPlayerManager.h"   // NF153: GetMainPlayer() 0x00449970 ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (Ego_r / FableWin; was the stand-in extern "C" GSI_GetWorld)

class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
extern "C" bool   __fastcall Hero_CheckChild(Hero* h);

bool __fastcall CGameScriptInterface_IsHeroChild(CGameScriptInterface* self)
{
    Hero* h = self->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (h != 0 && (h->flags & 1) == 0)
        return Hero_CheckChild(h);
    return false;
}