// CTCHeroRangedAttacks::GetProjectileTargetingZoomParameter() const -- returns the projectile-targeting zoom parameter (+0x40)
// Leaf: ?GetProjectileTargetingZoomParameter@CTCHeroRangedAttacks@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789A50, plain `ret` (float on the x87 stack): `fld dword ptr [ecx+40h] ; ret`.
// Identity (NF120, PRIORITY q4691): Ego_r.pdb / FableWin.pdb ?GetProjectileTargetingZoomParameter@CTCHeroRangedAttacks@@QBEMXZ (Ego_r
//   0x00790A30, just before GetPullBackAmount 0x00790A60 = retail 0x00789A70 and GetShotAccuracyFraction 0x00790A70 = retail 0x00789A80);
//   the body returns +0x40, Ego_r CTCHeroRangedAttacks::ProjectileWeaponZoomParameter. Was the descriptive stand-in C00789a50::M
//   (catalog _global::GetMember_float0x40_00789a50).
// Header: engine/CTCHeroRangedAttacks.h, the FABLE_RETAIL_FIRE_RANGED_WEAPON class (declares the getter; PDB layout, 0x4C bytes).
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroRangedAttacks.h"

float CTCHeroRangedAttacks::GetProjectileTargetingZoomParameter() const
{
    return ProjectileWeaponZoomParameter;
}
