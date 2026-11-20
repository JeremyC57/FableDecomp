// CPlayer::IsInMode17Or18() const -- true when the player's current mode is 0x11 or 0x12 (behavioural name)
// Leaf: IsInMode17Or18
// Retail: Fable.exe @ 0x004496B0, 36 bytes, unreferenced (no call, vtable or data reference in Fable.exe). Catalog label
// CAIStateGroup_IdleAtHome::InitCreaturePosition is wrong: the body calls CPlayer::GetCurrentMode (0x00633BE0, landed:
// this+0x210 PlayerModes list -> front mode) twice on the same this and tests 0x11 then 0x12, so it is a CPlayer
// const predicate. Ego_r.pdb CPlayer has several (IsSpellMode, IsExpressionShiftMode, IsAggressiveMode, ...); the
// EPlayerMode enumerator names are not in any local source, so the method and enumerators are named behaviourally.
// Layout: engine/CPlayer.h (GetCurrentMode is out of line in retail and declared only here).
// Shape: one `||` return expression (an if/return-true form or a `?:` chain is 3-4 bytes shorter).
// NF153: GetCurrentMode is the shared header's CPlayer member (FABLE_RETAIL_PLAYER: Ego_r / FableWin
//   ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ; was redeclared on the stand-in CPlayer_Methods), and the modes are the PDB
//   enumerators of engine/EPlayerMode.h (0x11 PLAYER_MODE_FREEZE_CONTROLS, 0x12 PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS; was a
//   local enum with made-up names). The predicate itself keeps its descriptive stand-in name (see NOTES).
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"
#include "engine/EPlayerMode.h"
struct CPlayer_Methods : CPlayer {
    bool IsInMode17Or18() const;
};
bool CPlayer_Methods::IsInMode17Or18() const
{
    return GetCurrentMode() == PLAYER_MODE_FREEZE_CONTROLS || GetCurrentMode() == PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS;
}