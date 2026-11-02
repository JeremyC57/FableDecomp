// CTCExpression::~CTCExpression() -- the compiler-generated destructor: release the PDef def pointer (+0xC), then ~CTCBase (tail jump)
// Leaf: ??1CTCExpression@@UAE@XZ
// Naming wave 31 (family tcd): FableWin.pdb ??1CTCExpression@@UAE@XZ; the slot-0 ??_G 0x004DC7AC of vtable 0x012401F4 (RTTI .?AVCTCExpression@@ : CTCBase) calls it.
//   Was "C3DAnimationBlendState::~C3DAnimationBlendState" over the invented non-virtual base BlendBase (??1BlendBase@@QAE@XZ bound to 0x00686830): the base is engine/CTCBase.h's retail
//   branch (family tcb), so the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DC7C8, 32 bytes, thiscall, tail `jmp ~CTCBase` (size-optimised TU: `and [esi+X],0`).
// Layout: Ego_r.pdb CTCExpression: PDef CDefPointer<const CExpressionDef> +0xC; the other members are trivially destructible.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCLightningOrb 0x004DCEDD); the def pointer
//   is engine/CDefPointer.h's (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line
//   implicit ??1 (chunk 656/657; wave 30 0x00A9D3A0).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                          // CDefPointer<T> (forced-inline Release)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830

class CExpressionDef;
class CTCExpression : public CTCBase {                    // vtable 0x012401F4
public:
    CDefPointer<const CExpressionDef> PDef;                // +0x0c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCExpression* object)
{
    object->CTCExpression::~CTCExpression();
}