// CPlayer::IsTargetLocked() const -- true while Z-targeting or either target lock is held
// Leaf: IsTargetLocked@CPlayer_Methods
// Retail: Fable.exe @ 0x004874A0, 35 bytes; Ego_r.pdb ?IsTargetLocked@CPlayer@@QBE_NXZ. Body: ZTargeting (+0x20a) ||
// ProjectileTargetLocked (+0x20b) || RightTriggerTargetLocked (+0x20c).
// Layout: engine/CPlayer.h (offsets as Ego_r.pdb).
// Shape: a nested `?:` chain ending in `? true : false` (first arm placed inline, `setne` on the last flag); `||` and
// if-chains lay the blocks out differently.
#include "engine/CPlayer.h"
struct CPlayer_Methods : CPlayer {
    bool IsTargetLocked() const;
};
bool CPlayer_Methods::IsTargetLocked() const
{
    return ZTargeting ? true : ProjectileTargetLocked ? true : RightTriggerTargetLocked ? true : false;
}