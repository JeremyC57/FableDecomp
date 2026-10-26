// CTCFlourishTarget::OnInitialActivate() -- when the parent thing is of type 1, register this component with it
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8A00, length 20 bytes (vtable 0x0123EF34 slot 6, the CTCBase
// "OnInitialActivate" slot; its twin 0x007E8A20 in slot 2 unregisters).
// Callees: CThing::SetComponentRegistered(bool, CTCBase*) @ 0x00833A10 (labelled "SetSuspendAnimation").
// Layout: CTCBase (engine/CTCBase.h) ParentThing +4; the thing's type byte at +0x10 (1 = the player creature, as the
// landed CThing_Retail models of the script interface).
// Shape: `this` copied to eax first (`mov eax,ecx`) so ecx can hold the thing; `cmp byte ptr [ecx+10h],1 ; jne`;
// `push eax ; push 1` and a plain call (a void call with a test before it: no tail jump since retail keeps `call ; ret`).
#include "engine/CTCBase.h"

class CThing { public:
    unsigned char _pad[0x10];
    unsigned char m_type;                          // +0x10
    void SetComponentRegistered(bool registered, CTCBase* component);   // 0x00833A10
};

class CTCFlourishTarget : public CTCBase { public:
    void OnInitialActivate();
};

void CTCFlourishTarget::OnInitialActivate()
{
    if (ParentThing->m_type == 1)
        ParentThing->SetComponentRegistered(true, this);
}