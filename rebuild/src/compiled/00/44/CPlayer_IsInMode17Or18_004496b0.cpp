// CPlayer::IsInMode17Or18() const -- true when the player's current mode is 0x11 or 0x12 (behavioural name)
// Leaf: IsInMode17Or18
// Retail: Fable.exe @ 0x004496B0, 36 bytes, unreferenced (no call, vtable or data reference in Fable.exe). Catalog label
// CAIStateGroup_IdleAtHome::InitCreaturePosition is wrong: the body calls CPlayer::GetCurrentMode (0x00633BE0, landed:
// this+0x210 PlayerModes list -> front mode) twice on the same this and tests 0x11 then 0x12, so it is a CPlayer
// const predicate. Ego_r.pdb CPlayer has several (IsSpellMode, IsExpressionShiftMode, IsAggressiveMode, ...); the
// EPlayerMode enumerator names are not in any local source, so the method and enumerators are named behaviourally.
// Layout: engine/CPlayer.h (GetCurrentMode is out of line in retail and declared only here).
// Shape: one `||` return expression (an if/return-true form or a `?:` chain is 3-4 bytes shorter).
#include "engine/CPlayer.h"
enum EPlayerMode { PLAYER_MODE_17 = 0x11, PLAYER_MODE_18 = 0x12 };
struct CPlayer_Methods : CPlayer {
    EPlayerMode GetCurrentMode() const;
    bool IsInMode17Or18() const;
};
bool CPlayer_Methods::IsInMode17Or18() const
{
    return GetCurrentMode() == PLAYER_MODE_17 || GetCurrentMode() == PLAYER_MODE_18;
}