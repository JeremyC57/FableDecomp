// CTCWeapon::ResetAiming() -- forget the aim: AimingTime back to -1 (not aiming) and clear the aiming-time override
// Leaf: ?ResetAiming@CTCWeapon@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DA4E0, 12 bytes (`mov dword [ecx+14h],-1 ; mov byte [ecx+18h],0 ; ret`).
// Name (agentNF25b, 2026-11-12): Ego_r.pdb + FableWin.pdb ?ResetAiming@CTCWeapon@@QAEXXZ. Ego_r donor order IsAimedForLongEnough,
//   SetAimingTimeOverride, SetAiming, ResetAiming, FrameUpdate, IsMeleeWeaponClass matches retail 0x005DA490 / 0x005DA4C0 (+0x18 = 1) /
//   0x005DA4D0 (+0x14: -1 -> 0) / 0x005DA4E0 / 0x005DA4F0 (FrameUpdate, landed) / 0x005DA510 (IsMeleeWeaponClass, landed); the body
//   writes Ego_r AimingTime +0x14 and AimingTimeOverride +0x18. Binders 0x006EA450 / 0x006ED510 / 0x008BF500 call it on a weapon.
//   Was sub_005da4e0 (a __fastcall free function over a made-up CGhostGrannySon model); ReadyProjectile (another binder's
//   spelling) is in neither PDB.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_RESETAIMING   // hdr overlay
#include "engine/CTCWeapon.h"

void CTCWeapon::ResetAiming()
{
    AimingTime = -1;
    AimingTimeOverride = false;
}
