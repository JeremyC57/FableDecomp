// CTCSmashableBase::~CTCSmashableBase() -- the compiler-generated destructor: release the Definition def pointer (+0x14), then ~CTCBase (tail jump)
// Leaf: ??1CTCSmashableBase@@UAE@XZ
// Naming wave 31 (family tcd): FableWin.pdb ??1CTCSmashableBase@@UAE@XZ; the slot-0 ??_G 0x007EEF60 of vtable 0x0127185C (RTTI .?AVCTCSmashableBase@@ : CTCBase) calls it, and the ??_G of CTCSmashableWindow 0x004DC6E2 / CTCSmashableBarrier 0x004DC71B (RTTI : CTCSmashableBase).
//   Was "C3DAnimationBlendState::~C3DAnimationBlendState" over the invented non-virtual base CBase3DAnimState (??1CBase3DAnimState@@QAE@XZ bound to 0x00686830): the base is engine/CTCBase.h's retail
//   branch (family tcb), so the tail is ??1CTCBase@@UAE@XZ 0x00686830 (Ego_r.pdb / FableWin.pdb).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB004, 32 bytes, thiscall, tail `jmp ~CTCBase` (size-optimised TU: `and [esi+X],0`).
// Layout: Ego_r.pdb CTCSmashableBase: Definition CDefPointer<const CSmashableDef> +0x14; before it LastWorldFrameHit +0x0c, Smashed +0x10; the other members are trivially destructible.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCLightningOrb 0x004DCEDD); the def pointer
//   is engine/CDefPointer.h's (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line
//   implicit ??1 (chunk 656/657; wave 30 0x00A9D3A0).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                          // CDefPointer<T> (forced-inline Release)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                              // : CBaseClassNonCopyable, vtable 0x0125BE0C; ~CTCBase 0x00686830

class CSmashableDef;
class CTCSmashableBase : public CTCBase {                 // vtable 0x0127185C
public:
    unsigned char _pad_0x0c[0x8];                                   // +0x0c LastWorldFrameHit +0x0c, Smashed +0x10
    CDefPointer<const CSmashableDef> Definition;           // +0x14
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCSmashableBase* object)
{
    object->CTCSmashableBase::~CTCSmashableBase();
}