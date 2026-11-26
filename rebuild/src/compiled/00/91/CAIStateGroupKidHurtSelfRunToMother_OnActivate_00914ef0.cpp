// CAIStateGroup_KidHurtSelfRunToMother::OnActivate() -- on activation the kid has not yet added its sad mode: AddedSadMode = false
// Leaf: ?OnActivate@CAIStateGroup_KidHurtSelfRunToMother@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00914EF0, thiscall (`mov byte ptr [ecx+14h],0 ; ret`); vftable 0x01289824 slot 2 (its only retail reference).
// Identity (NF475, r467b unit with ctor 0x008FD9DA): RTTI .?AVCAIStateGroup_KidHurtSelfRunToMother@@ vtable 0x01289824 slot 2 = 0x00914EF0 +
//   Ego_r.pdb / FableWin.pdb ?OnActivate@CAIStateGroup_KidHurtSelfRunToMother@@UAEXXZ (public override). Was the stand-in class
//   ?OnActivate@CAIStateGroup_KidHurtSelfRunToMother_Methods@@QAEXXZ (T3 alias1 / nf318: the class view existed only under
//   FABLE_RETAIL_KID_HURT_SELF_PASS23; it is the view the ctor row 0x008FD9DA and the landed siblings 0x00915190 / 0x00915350 compile).
// Model: engine/CAIStateGroup_KidHurtSelfRunToMother.h retail view FABLE_RETAIL_KID_HURT_SELF_PASS23 (the ctor row's class).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_KID_HURT_SELF_PASS23
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"

void CAIStateGroup_KidHurtSelfRunToMother::OnActivate()
{
    AddedSadMode = false;
}
