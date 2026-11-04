// NUISystem::CViewport::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x0054EC70
// Leaf: ??_ECViewport@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054EC60, 8 bytes: `sub ecx,18h ; jmp 0x0054EC70`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CViewport's CActionParam vtable 0x01249A30 at +0x18 (rtti-vtables.tsv, RTTI .?AVCViewport@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0054EC70 is slot 0 of the primary vtable 0x01249A5C, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CViewport(long) (0x0054EA00: CChangingStateComponent(long) 0x0052CC50, the vptrs, the Handle and FakeChildren members, GraphicIndex cleared, GetDataFromDef 0x0054E5D0).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCViewport@NUISystem@@UAEPAXI@Z 0x0054EC70 (bound as its ??_E alias).
// Header: engine/NUISystem_CViewport.h.
// Twin: 0x0054EC50 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CViewport.h"
namespace NUISystem {
CViewport::CViewport(long def)
    : CChangingStateComponent(def)
    , GraphicIndex(0)
{
    GetDataFromDef();
}
}