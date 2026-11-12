// CGameScriptInterface::HeroGoDigging() -- the main player enters PLAYER_MODE_DIGGING (30), not exclusive.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088DF80: mov ecx,[ecx+14h] ; call 0x00449970 ; push 0 ; push 1Eh ; mov ecx,eax ;
// call 0x006345C0 ; ret.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ, engine/CPlayerManager.h)
// and CPlayer::AddMode 0x006345C0 (?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z, Ego_r.pdb 0x00554BE0 + FableWin.pdb,
// engine/CPlayer.h). PLAYER_MODE_DIGGING = 30 is engine/EPlayerMode.h (both PDBs). The earlier stand-ins
// @FableHeroResolve_88df80@4 / ?GoDigging@CDigTarget@@QAEXHH@Z respelled (agentNF27, 2026-11-12).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void HeroGoDigging();
};

void CGameScriptInterface_Methods::HeroGoDigging()
{
    CPlayer& player = PlayerManager->GetMainPlayer();   // a named local: the lookup is sequenced before the pushes
    player.AddMode(PLAYER_MODE_DIGGING, false);
}
