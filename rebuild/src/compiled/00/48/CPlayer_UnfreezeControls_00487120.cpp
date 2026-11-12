// CPlayer::UnfreezeControls() -- RemoveMode(PLAYER_MODE_FREEZE_CONTROLS), then clears the player interface's Suspended flag
// Leaf: ?UnfreezeControls@CPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487120, thiscall, `push 11h ; call RemoveMode 0x00634190 ; mov eax,[esi+0Ch] ; mov byte [eax+79Ch],0`.
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00487120, the stand-in CHTG_Executioner::OnPredicateFail): Ego_r.pdb and FableWin.pdb both list
// ?UnfreezeControls / ?FreezeControls / ?SpecialAbilityUnfreezeControls / ?SpecialAbilityFreezeControls@CPlayer@@QAEXXZ in that order
// (Ego_r 0x00492870 / 0x00492890 / 0x004928A0 / 0x004928C0); retail keeps the order (0x00487120 / 0x00487140 / 0x00487150 / 0x00487170),
// the Freeze pair calling AddMode 0x006345C0 and the Unfreeze pair RemoveMode 0x00634190 with EPlayerMode 17 (PLAYER_MODE_FREEZE_CONTROLS)
// / 18 (PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS). +0x0C is CPlayer::PlayerInterface, +0x79C CGamePlayerInterface::Suspended (Ego_r).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_PLAYER_FREEZECONTROLS
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
void CPlayer::UnfreezeControls()
{
    RemoveMode(PLAYER_MODE_FREEZE_CONTROLS);
    PlayerInterface->Suspended = false;
}
