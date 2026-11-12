// CTCWeapon::SetAimingTimeOverride() -- force the aim to count as held long enough: AimingTimeOverride = true
// Leaf: ?SetAimingTimeOverride@CTCWeapon@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DA4C0, 5 bytes (`mov byte [ecx+18h],1 ; ret`).
// Name (agentNF26, 2026-11-12): Ego_r.pdb + FableWin.pdb ?SetAimingTimeOverride@CTCWeapon@@QAEXXZ. Ego_r donor order IsAimedForLongEnough,
//   SetAimingTimeOverride, SetAiming, ResetAiming, FrameUpdate matches retail 0x005DA490 (reads +0x18 / +0x14, `ret 4`) / 0x005DA4C0 /
//   0x005DA4D0 (+0x14: -1 -> 0) / 0x005DA4E0 (ResetAiming, landed) / 0x005DA4F0 (FrameUpdate, landed); the body stores Ego_r
//   AimingTimeOverride +0x18. Binder 0x007E5B90 (CTCFireRangedWeaponOnLeavingStateGroup::FrameUpdate) calls it on the loaded weapon.
//   Was CXMVPlaybackInfo::CXMVPlaybackInfo (a propagated label; the source graded a stand-in C005da4c0::M); SetFireRequested (the
//   binder's spelling) is in neither PDB.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_AIMING   // hdr overlay (agentNF26)
#include "engine/CTCWeapon.h"

void CTCWeapon::SetAimingTimeOverride()
{
    AimingTimeOverride = true;
}
