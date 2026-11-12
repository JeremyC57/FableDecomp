// CPlayer::FreezeControls() -- AddMode(PLAYER_MODE_FREEZE_CONTROLS, false)
// Leaf: ?FreezeControls@CPlayer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487140, 10 bytes, thiscall: `push 0 ; push 17 ; call AddMode 0x006345C0 ; ret` (ecx passed through).
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was _unattributed CClass_00487140::CallWith2_00487140, a shape-generated stand-in):
//   Ego_r.pdb (0x00492890) + FableWin.pdb (0x01ACAAC0) ?FreezeControls@CPlayer@@QAEXXZ, the second of the four of the Unfreeze / Freeze / SpecialAbilityUnfreeze /
//   SpecialAbilityFreeze block in both PDBs, retail 0x00487120 / 0x00487140 / 0x00487150 / 0x00487170 (FAM10 named the two Unfreeze
//   bodies, RemoveMode of the same modes); the callee is ?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z (engine/CPlayer.h), EPlayerMode 17 =
//   PLAYER_MODE_FREEZE_CONTROLS (engine/EPlayerMode.h).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_FREEZECONTROLS
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
void CPlayer::FreezeControls()
{
    AddMode(PLAYER_MODE_FREEZE_CONTROLS, false);
}
