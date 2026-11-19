// CTCFlourishTarget::OnRemove() -- when the parent thing is of type 1, unregister this component from it
// Leaf: ?OnRemove@CTCFlourishTarget@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8A20, length 20 bytes (vtable 0x0123EF34 slot 2, CTCBase's OnRemove slot
// (engine/CTCBase.h); its twin 0x007E8A00 in slot 6 registers).
// Name: Ego_r.pdb ?OnInitialActivate@CTCFlourishTarget@@UAEXXZ 0x007E3860 / ?OnRemove@CTCFlourishTarget@@UAEXXZ 0x007E3880 mirror retail
//   0x007E8A00 / 0x007E8A20, and Ego_d tc_flourish_target.obj lists OnInitialActivate, ProcessActionMovementDelta, OnRemove (no
//   FinalCleanup). NF95 2026-11-19 (q11294): was the non-virtual stand-in FinalCleanup.
// Callees: CThingAICreature::SuspendAI(bool, CTCBase*) @ 0x00833A10 (labelled "SetSuspendAnimation").
// Layout: CTCBase (engine/CTCBase.h) ParentThing +4; the thing's type byte at +0x10 (1 = the player creature, as the
// landed CThing_Retail models of the script interface).
// Shape: `this` copied to eax first (`mov eax,ecx`) so ecx can hold the thing; `cmp byte ptr [ecx+10h],1 ; jne`;
// `push eax ; push 0` and a plain call (a void call with a test before it: no tail jump since retail keeps `call ; ret`).
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

#include "engine/CThing.h"

class CTCFlourishTarget : public CTCBase { public:
    virtual void OnRemove();
};

void CTCFlourishTarget::OnRemove()
{
    if (ParentThing->GetType() == 1)
        ((CThingAICreature*)ParentThing)->SuspendAI(false, this);
}