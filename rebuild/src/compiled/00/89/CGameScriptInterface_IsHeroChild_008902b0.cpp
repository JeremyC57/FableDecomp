// Leaf: ?IsHeroChild@CGameScriptInterface@@UBE_NXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Hero {
    char pad[0x91];
    unsigned char flags;
};
class CThingPlayerCreature : public Hero { };

#include "engine/CPlayerManager.h"   // NF153: GetMainPlayer() 0x00449970 ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (Ego_r / FableWin; was the stand-in extern "C" GSI_GetWorld)

class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
extern "C" bool   __fastcall Hero_CheckChild(Hero* h);

bool CGameScriptInterface::IsHeroChild() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    Hero* h = self->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (h != 0 && (h->flags & 1) == 0)
        return Hero_CheckChild(h);
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_IsHeroChild -> CGameScriptInterface::IsHeroChild, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
