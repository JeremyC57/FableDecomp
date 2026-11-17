struct CResult {
    bool Method2(int n);
};
class CPlayer;
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its PPlayerManager scoped_ptr +0x1C through the inline GetPlayerManager() (FableWin.pdb ?GetPlayerManager@CMainGameComponent@@QAEAAVCPlayerManager@@XZ); was the stand-in CGlobal* g_global (and its CSub stand-in).
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#include "engine/CMainGameComponent.h"

int __fastcall CGameScriptInterface_IsHeroInTavernGame(void* self)
{
    (void)self;
    CResult* r = ((CResult*)&CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer());
    if (r && !r->Method2(0x1a))
        return 0;
    return 1;
}