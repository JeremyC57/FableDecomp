// CAIStateGroup_Freeze::Update() -- the frozen creature does nothing until the group can no longer run, then terminates
// Leaf: ?Update@CAIStateGroup_Freeze@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009224F0, 34 bytes; vftable 0x0128A95C slot 12 (its only retail reference, 0x0128A98C).
// Callees: CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, the virtual IsRunnable (slot 11, `call [eax+2Ch]`), CAIStateGroupBase::Terminate
//   @ 0x008FCF40, tail-jumped.
// Shape: `while (AIAction_DoNothing() == AI_ACTION_IN_PROGRESS (4) && IsRunnable()) ;` then Terminate().
// NF473 (r467b / r469c respell, unit with ctor 0x008FE593 + ??_G 0x0090194E): the self is the PDB spelling ?Update@CAIStateGroup_Freeze@@UAEXXZ
//   (Ego_r.pdb + FableWin.pdb name sets; public virtual, slot 12 of the RTTI .?AVCAIStateGroup_Freeze@@ vftable), was the non-virtual
//   ?Update@CAIStateGroup_Freeze@@QAEXXZ. The class's base comes from the shared header engine/CAIStateGroupBase.h (was a local model with
//   v00..v10 placeholder slots); no engine header exists for CAIStateGroup_Freeze, so the class is declared as the ctor row 0x008FE593 declares it.
#include "engine/fable_ai_action_enums.h"
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"

class CAIStateGroup_Freeze : public CAIStateGroupBase {
public:
    CAIStateGroup_Freeze();                                                 // 0x008FE593
    virtual bool IsRunnable();                                                     // slot 11 0x009224E0
    virtual void Update();                                                         // slot 12 0x009224F0
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_Freeze) == 0x14);

void CAIStateGroup_Freeze::Update()
{
    while (AIAction_DoNothing() == AI_ACTION_IN_PROGRESS && IsRunnable())
        ;
    Terminate();
}
