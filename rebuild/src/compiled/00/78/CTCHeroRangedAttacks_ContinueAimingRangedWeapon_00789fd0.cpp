// CTCHeroRangedAttacks::ContinueAimingRangedWeapon() -- while the hero can still use the ranged weapon, keep the fire cycle and the aim going
// Leaf: ?ContinueAimingRangedWeapon@CTCHeroRangedAttacks@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789FD0, 22 bytes: `call 0x00789E60 ; test al,al ; je ; mov al,1 ; mov [esi+14h],al ;
//   mov [esi+15h],al`.
// NF167 (2026-11-20, q11689): self rename + port. Was the legacy free `ContinueAimingRangedWeapon(CTCHeroRangedAttacks*)` on a row-local
//   CTCHeroRangedAttacks model, calling 0x00789E60 through a fake same-TU static `Helper` body (under `#pragma optimize("", off)`).
// Identity: Ego_r.pdb 0x00791100 / FableWin.pdb 0x024F5D40 ?ContinueAimingRangedWeapon@CTCHeroRangedAttacks@@QAEXXZ (public; catalog name);
//   the callee is the private const ?IsAbleToUseRangedWeapon@CTCHeroRangedAttacks@@ABE_NXZ (Ego_r 0x00790F50 / FableWin 0x024F5500), and
//   FireCycleInProgress / Aiming are the header's bools at +0x14 / +0x15 (engine/CTCHeroRangedAttacks.h FABLE_RETAIL_FIRE_RANGED_WEAPON).
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroRangedAttacks.h"

void CTCHeroRangedAttacks::ContinueAimingRangedWeapon()
{
    if (IsAbleToUseRangedWeapon()) {
        FireCycleInProgress = true;
        Aiming = true;
    }
}
