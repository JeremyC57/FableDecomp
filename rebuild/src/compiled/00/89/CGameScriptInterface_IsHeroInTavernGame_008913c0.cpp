struct CResult {
    bool Method2(int n);
};
class CPlayer;
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
struct CSub {

};
struct CGlobal {
    char pad[0x1c];
    CSub* sub;   // at +0x1c
};

extern CGlobal* g_global;

int __fastcall CGameScriptInterface_IsHeroInTavernGame(void* self)
{
    (void)self;
    CResult* r = ((CResult*)&((CPlayerManager*)g_global->sub)->GetMainPlayer());
    if (r && !r->Method2(0x1a))
        return 0;
    return 1;
}