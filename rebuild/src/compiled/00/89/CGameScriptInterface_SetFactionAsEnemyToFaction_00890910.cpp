#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Faction {
    void AddEnemy(Faction* other);
};
struct FactionMgr {
    Faction* Lookup(int id);
};
class CWorld { public: char pad[0x54]; FactionMgr* mgr; };

void __fastcall CGameScriptInterface_SetFactionAsEnemyToFaction(CGameScriptInterface* self, int dummy, int faction1, int faction2)
{
    FactionMgr* m = self->World->mgr;
    Faction* fa = m->Lookup(faction1);
    FactionMgr* m2 = self->World->mgr;
    Faction* fb = m2->Lookup(faction2);
    if (fa && fb) {
        fa->AddEnemy(fb);
        fb->AddEnemy(fa);
    }
}