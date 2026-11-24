// CPlayer::IsTargetLocked() const -- true while Z-targeting or either target lock is held
// Leaf: ?IsTargetLocked@CPlayer@@QBE_NXZ
// Retail: Fable.exe @ 0x004874A0, 35 bytes; Ego_r.pdb ?IsTargetLocked@CPlayer@@QBE_NXZ. Body: ZTargeting (+0x20a) ||
// ProjectileTargetLocked (+0x20b) || RightTriggerTargetLocked (+0x20c).
// Layout: engine/CPlayer.h (offsets as Ego_r.pdb).
// Shape: a nested `?:` chain ending in `? true : false` (first arm placed inline, `setne` on the last flag); `||` and
// if-chains lay the blocks out differently.
// Name (NF378): Ego_r.pdb / FableWin.pdb ?IsTargetLocked@CPlayer@@QBE_NXZ; BSim 1.0 and Ego_r player.obj order (was the stand-in
// CPlayer_Methods). Declared by the engine/CPlayer.h opt-in below.
#define FABLE_RETAIL_CPLAYER_ISTARGETLOCKED
#include "engine/CPlayer.h"
bool CPlayer::IsTargetLocked() const
{
    return ZTargeting ? true : ProjectileTargetLocked ? true : RightTriggerTargetLocked ? true : false;
}