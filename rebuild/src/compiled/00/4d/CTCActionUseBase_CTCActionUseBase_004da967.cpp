// CTCActionUseBase::~CTCActionUseBase() -- the compiler-generated destructor: release the PActionUseDef def pointer (+0xC), then ~CTCBase (tail jump)
// Leaf: ??1CTCActionUseBase@@UAE@XZ
// Naming wave 31 (family tcd): FableWin.pdb ??1CTCActionUseBase@@UAE@XZ; the slot-0 ??_G 0x004DBF76 of vtable 0x0123F09C (RTTI .?AVCTCActionUseBase@@ : CTCBase) calls it, and the implicit destructors / ??_G of its derived CTCActionUse* / CTCCarriedActionUse* classes (RTTI : CTCActionUseBase).
//   Was "C3DAnimationBlendState::~C3DAnimationBlendState" over the invented non-virtual base BlendStateBase (??1BlendStateBase@@QAE@XZ bound to 0x00686830): the base is engine/CTCBase.h's retail
//   branch (family tcb), so the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA967, 32 bytes, thiscall, tail `jmp ~CTCBase` (size-optimised TU: `and [esi+X],0`).
// Layout: Ego_r.pdb CTCActionUseBase: PActionUseDef CDefPointer<const CActionUseDef> +0xC; the other members are trivially destructible.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCLightningOrb 0x004DCEDD); the def pointer
//   is engine/CDefPointer.h's (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line
//   implicit ??1 (chunk 656/657; wave 30 0x00A9D3A0).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                          // CDefPointer<T> (forced-inline Release)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830

class CActionUseDef;
class CTCActionUseBase : public CTCBase {                 // vtable 0x0123F09C
public:
    CDefPointer<const CActionUseDef> PActionUseDef;        // +0x0c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCActionUseBase* object)
{
    object->CTCActionUseBase::~CTCActionUseBase();
}