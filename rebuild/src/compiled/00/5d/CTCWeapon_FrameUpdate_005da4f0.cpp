// CTCWeapon::FrameUpdate() -- counts the frames the weapon has been wielded and, while aiming, the frames spent aiming
// Leaf: ?FrameUpdate@CTCWeapon@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DA4F0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 9 of CTCWeapon's vtable 0x012554FC (the slot holds this body in 1 vtable(s), owner CTCWeapon); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCWeapon@@UAEXXZ; `ret` matches the signature.
// Was: CTCWeapon::vfunc_9 (rebuild/src/compiled/00/5d/CTCWeapon_FrameUpdate_005da4f0.cpp).
#define FABLE_RETAIL_CTCWEAPON_VSLOTS3
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#include "engine/CTCWeapon.h"
void CTCWeapon::FrameUpdate()
{
    if (Wielded)
        ++TimeWielded;
    if (AimingTime != -1)
        ++AimingTime;
}
