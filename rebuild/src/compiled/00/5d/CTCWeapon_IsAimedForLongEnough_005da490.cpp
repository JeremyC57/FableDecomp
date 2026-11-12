// CTCWeapon::IsAimedForLongEnough(float) const -- whether the weapon has been aimed for longer than the given time: true at once when
// AimingTimeOverride is set, else AimingTime (frames) over CMainGameComponent::ConstantFPS compared with the argument
// Leaf: ?IsAimedForLongEnough@CTCWeapon@@QBE_NM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DA490, 37 bytes, thiscall, `ret 4`.
// Name (agentNF26, 2026-11-12): Ego_r.pdb + FableWin.pdb ?IsAimedForLongEnough@CTCWeapon@@QBE_NM@Z. Ego_r donor order IsAimedForLongEnough,
//   SetAimingTimeOverride, SetAiming, ResetAiming, FrameUpdate matches retail 0x005DA490 / 0x005DA4C0 / 0x005DA4D0 / 0x005DA4E0 /
//   0x005DA4F0; the body reads Ego_r AimingTimeOverride +0x18 and AimingTime +0x14 (`fild` / `fidiv` ConstantFPS) against the float
//   argument (`ret 4`). Was global Comparison_Float_WithEpsilon (a __fastcall free function over a made-up Obj model); retail has no
//   direct caller of it in the catalog.
// Naming wave 18: the divisor 0x01375550 is the protected static long CMainGameComponent::ConstantFPS, read through the inline static
//   accessor GetConstantFPS() (Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ); now from engine/CMainGameComponent.h.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_AIMING   // hdr overlay (agentNF26)
#include "engine/CTCWeapon.h"
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CMainGameComponent.h"

bool CTCWeapon::IsAimedForLongEnough(float time) const
{
    if (AimingTimeOverride) return true;
    if ((float)AimingTime / CMainGameComponent::GetConstantFPS() > time) return true;
    return false;
}
