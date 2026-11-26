// CAIStateGroup_MinionKidCalledToMother::IsRunnable() -- runnable while the kid's mother has a "call home" message out for it
// Leaf: ?IsRunnable@CAIStateGroup_MinionKidCalledToMother@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090EEB0, 12 bytes; vftable 0x01288F48 slot 11 (its only retail reference, 0x01288F74).
// Retail bytes:  call 0x0090EE20 ; neg eax ; sbb eax,eax ; neg eax ; ret
// NF473 (r467b / r469c respell, unit with ctor 0x008FD459 + ??_G 0x008FEF6F; T3 nf319 / nf322 / nF325 resolved): the self is the PDB spelling
//   ?IsRunnable@CAIStateGroup_MinionKidCalledToMother@@UAE_NXZ (Ego_r.pdb + FableWin.pdb name sets; public virtual bool, slot 11 of the RTTI
//   .?AVCAIStateGroup_MinionKidCalledToMother@@ vftable 0x01288F48), was ?IsRunnable@CAIStateGroup_MinionKidCalledToMother_Methods@@QAE_NXZ on a
//   `_Methods` alias struct over the flat header (nf319). The class comes from the shared header's retail branch (engine/CAIStateGroup_MinionKidCalledToMother.h),
//   and the callee 0x0090EE20 is the private ?FindCallHomeEvent@CAIStateGroup_MinionKidCalledToMother@@AAEPBVCMessageEvent@@XZ of both PDBs (nf322; the
//   header's FINDCALLHOMEEVENT opt-in), was ?FindCallHomeEvent@CAIStateGroup_MinionKidCalledToMother_Methods@@QAEPAVCMessageEvent@@XZ.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDCALLEDTOMOTHER
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDCALLEDTOMOTHER_FINDCALLHOMEEVENT
#include "engine/CAIStateGroup_MinionKidCalledToMother.h"

bool CAIStateGroup_MinionKidCalledToMother::IsRunnable()
{
    return FindCallHomeEvent() != 0;
}
