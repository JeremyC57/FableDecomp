// CTCForcePushed::OnKill() -- when the pushed thing is alive and of type 1, unregister this component from it
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798510, length 34 bytes (vtable 0x0126C09C slot 14, the CTCBase
// "OnKill" slot).
// Callees: CThing::SetComponentRegistered(bool, CTCBase*) @ 0x00833A10 (labelled "SetSuspendAnimation").
// Layout: CTCBase (engine/CTCBase.h) ParentThing +4; the thing's type byte +0x10 and dead bit (bit 0 of +0x91, the
// landed IS_THING_DEAD 0x00836BE0).
// Shape: ParentThing loaded straight into ecx and null-tested; the dead-bit test materialises the 1 once
// (`mov al,1 ; test al,dl`) and reuses it for `cmp byte ptr [ecx+10h],al` -- the bit test is written `& 1` over the
// byte and the type compared with the literal 1; `push 0 ; push 0`: the component argument is a null pointer here.
#include "engine/CTCBase.h"

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10
    unsigned char _pad11[0x80];
    unsigned char m_flags;                         // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    void SetComponentRegistered(bool registered, CTCBase* component);   // 0x00833A10
};

class CTCForcePushed : public CTCBase { public:
    void OnKill();
};

void CTCForcePushed::OnKill()
{
    CThing* thing = ParentThing;
    if (thing && !thing->IsDead() && thing->m_type == 1)
        thing->SetComponentRegistered(false, 0);
}