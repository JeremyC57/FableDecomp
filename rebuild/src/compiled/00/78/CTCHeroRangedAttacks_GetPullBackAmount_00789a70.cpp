// CTCHeroRangedAttacks::GetPullBackAmount() const -- returns the bow pull-back amount (+0x20)
// Leaf: ?GetPullBackAmount@CTCHeroRangedAttacks@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789A70, plain `ret` (float on the x87 stack): `fld dword ptr [ecx+20h] ; ret`.
// Identity (NF120, PRIORITY q6194): Ego_r.pdb 0x00790A60 / FableWin.pdb 0x024F96A0 ?GetPullBackAmount@CTCHeroRangedAttacks@@QBEMXZ (Ego_r
//   order: GetPullBackAmount 0x00790A60 then GetShotAccuracyFraction 0x00790A70 = the landed retail 0x00789A80); the body returns +0x20,
//   Ego_r CTCHeroRangedAttacks::PullBackAmount; callers fetch interface 0x81 (TCI_HERO_RANGED_ATTACKS) first (0x00803581, 0x008EA220).
//   Was the descriptive stand-in C00789a70::M (manifest "CMap::DrawGetSeaLevel" propagated).
// Header: engine/CTCHeroRangedAttacks.h, the FABLE_RETAIL_FIRE_RANGED_WEAPON class (declares the getter; PDB layout, 0x4C bytes).
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroRangedAttacks.h"

float CTCHeroRangedAttacks::GetPullBackAmount() const
{
    return PullBackAmount;
}
