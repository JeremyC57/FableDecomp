// CTCWeapon::FrameUpdate() -- count the frames the weapon has been wielded and, while aiming, the frames spent aiming
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DA4F0, length 23 bytes (+ int3 padding).
// CTCWeapon vtable 0x012554FC slot 9 (rtti-vtables.tsv; chunk label "CTCWeapon::vfunc_9"); slot 9 of the CTCBase-derived
// vtables is FrameUpdate.
// Layout: engine/CTCWeapon.h (Ego_r.pdb): AimingTime +0x14 (-1 = not aiming), TimeWielded +0x20, Wielded +0x24.
// Shape: the wielded counter is a memory `inc`, the aiming counter is loaded, compared with -1 and stored back.
#include "engine/CTCWeapon.h"

struct CTCWeapon_FrameUpdate : CTCWeapon {
    void FrameUpdate();
};

void CTCWeapon_FrameUpdate::FrameUpdate()
{
    if (Wielded)
        ++TimeWielded;
    if (AimingTime != -1)
        ++AimingTime;
}