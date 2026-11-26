// CAIStateGroup_StandStill::Update() -- do nothing for as long as the do-nothing action stays in progress
// Leaf: ?Update@CAIStateGroup_StandStill@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091BCF0, 17 bytes; vftable 0x01289D88 slot 12 (its only retail reference).
// Retail bytes:  push esi ; mov esi,ecx ; L: mov ecx,esi ; call 0x0097E200 ; cmp eax,4 ; je L ; pop esi ; ret
// NF473 (r467b / r469c respell, unit with ctor 0x008FDE1B + ??_G 0x00900484): the self is the PDB spelling ?Update@CAIStateGroup_StandStill@@UAEXXZ
//   (Ego_r.pdb + FableWin.pdb name sets; public virtual, slot 12 of the RTTI .?AVCAIStateGroup_StandStill@@ vftable 0x01289D88), was the
//   non-virtual ?Update@CAIStateGroup_StandStill@@QAEXXZ of a one-int local model. The class and its base come from the shared headers
//   (engine/CAIStateGroup_StandStill.h); the callee 0x0097E200 is the protected base member
//   ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ (Ego_r.pdb; engine/CAIStateGroupBase.h), was the free stand-in
//   ?ss_sub@@YIHPAUCAIStateGroup_StandStill@@@Z. 4 = AI_ACTION_IN_PROGRESS (the PDB enum, engine/fable_ai_action_enums.h).
#include "engine/fable_ai_action_enums.h"
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_STANDSTILL
#include "engine/CAIStateGroup_StandStill.h"

void CAIStateGroup_StandStill::Update()
{
    while (AIAction_DoNothing() == AI_ACTION_IN_PROGRESS)
        ;
}
