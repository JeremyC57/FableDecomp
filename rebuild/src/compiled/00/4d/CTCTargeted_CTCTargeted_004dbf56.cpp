// CTCTargeted::~CTCTargeted() -- the compiler-generated destructor: release the PTargetingDef def pointer (+0x14), then ~CTCBase (tail jump)
// Leaf: ??1CTCTargeted@@UAE@XZ
// Name (agentNF20, 2026-11-11): Ego_r.pdb + FableWin.pdb ??1CTCTargeted@@UAE@XZ; its only caller is the slot-0 ??_G 0x004DBF3A of the
//   CTCTargeted vtable (rtti-vtables.tsv; RTTI .?AVCTCTargeted@@ : CTCBase). Was "C3DAnimationBlendState::~C3DAnimationBlendState" over
//   the invented non-virtual base BlendBase (??1BlendBase@@QAE@XZ bound to 0x00686830): the base is engine/CTCBase.h's retail branch, so
//   the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DBF56, 32 bytes, thiscall, tail `jmp ~CTCBase` (size-optimised TU: `and [esi+X],0`).
// Layout: the shared engine/CTCTargeted.h (Ego_r.pdb CTCTargeted: PTargetingDef CDefPointer<const CTargetingDef> +0x14; the other
//   members are trivially destructible).
// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCExpression 0x004DC7C8); the def pointer
//   is engine/CDefPointer.h's (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line
//   implicit ??1.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTARGETED
#include "engine/CTCTargeted.h"

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCTargeted* object)
{
    object->CTCTargeted::~CTCTargeted();
}
