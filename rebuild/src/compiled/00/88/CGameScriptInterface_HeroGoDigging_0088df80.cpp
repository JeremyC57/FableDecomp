// Leaf: ?HeroGoDigging@CGameScriptInterface@@UBEXXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h (class-model switch FABLE_RETAIL_NSCRIPT; was ?HeroGoDigging@CGameScriptInterface_Methods@@QAEXXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// CGameScriptInterface::HeroGoDigging() const -- the main player enters PLAYER_MODE_DIGGING (30), not exclusive.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088DF80: mov ecx,[ecx+14h] ; call 0x00449970 ; push 0 ; push 1Eh ; mov ecx,eax ;
// call 0x006345C0 ; ret.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ, engine/CPlayerManager.h)
// and CPlayer::AddMode 0x006345C0 (?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z, Ego_r.pdb 0x00554BE0 + FableWin.pdb,
// engine/CPlayer.h). PLAYER_MODE_DIGGING = 30 is engine/EPlayerMode.h (both PDBs). The earlier stand-ins
// @FableHeroResolve_88df80@4 / ?GoDigging@CDigTarget@@QAEXHH@Z respelled (agentNF27, 2026-11-12).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PLAYER
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/EPlayerMode.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"

void CGameScriptInterface::HeroGoDigging() const
{
    CPlayer& player = PlayerManager->GetMainPlayer();   // a named local: the lookup is sequenced before the pushes
    player.AddMode(PLAYER_MODE_DIGGING, false);
}
