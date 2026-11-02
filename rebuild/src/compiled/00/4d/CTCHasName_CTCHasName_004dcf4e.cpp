// CTCHasName::~CTCHasName() -- the compiler-generated destructor: release the PDef def pointer (+0xC), then ~CTCBase (tail jump)
// Leaf: ??1CTCHasName@@UAE@XZ
// Naming wave 31 (family tcd): FableWin.pdb ??1CTCHasName@@UAE@XZ; the slot-0 ??_G 0x004DCF32 of vtable 0x01240AA4 (RTTI .?AVCTCHasName@@ : CTCBase) calls it.
//   Was "C3DAnimationBlendState::~C3DAnimationBlendState" over the invented non-virtual base Base (??1Base@@QAE@XZ bound to 0x00686830): the base is engine/CTCBase.h's retail
//   branch (family tcb), so the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCF4E, 32 bytes, thiscall, tail `jmp ~CTCBase` (size-optimised TU: `and [esi+X],0`).
// Layout: Ego_r.pdb CTCHasName: PDef CDefPointer<const CHasNameDef> +0xC; the other members are trivially destructible.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCLightningOrb 0x004DCEDD); the def pointer
//   is engine/CDefPointer.h's (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line
//   implicit ??1 (chunk 656/657; wave 30 0x00A9D3A0).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                          // CDefPointer<T> (forced-inline Release)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830

class CHasNameDef;
class CTCHasName : public CTCBase {                       // vtable 0x01240AA4
public:
    CDefPointer<const CHasNameDef> PDef;                   // +0x0c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCHasName* object)
{
    object->CTCHasName::~CTCHasName();
}