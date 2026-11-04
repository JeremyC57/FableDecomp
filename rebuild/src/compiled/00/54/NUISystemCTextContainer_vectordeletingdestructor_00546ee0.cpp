// NUISystem::CTextContainer::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00546F00
// Leaf: ??_ECTextContainer@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546EE0, 8 bytes: `sub ecx,4 ; jmp 0x00546F00`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CTextContainer's CObserver vtable 0x0124803C at +4 (rtti-vtables.tsv, RTTI .?AVCTextContainer@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00546F00 is slot 0 of the primary vtable 0x01248064, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CTextContainer(long) (0x00546D30: CChangingStateComponent(long) 0x0052CC50, the vptrs, GetDataFromDef 0x00546CD0).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCTextContainer@NUISystem@@UAEPAXI@Z 0x00546F00 (bound as its ??_E alias).
// Header: engine/NUISystem_CTextContainer.h.
// Twin: 0x00546EF0 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CTextContainer.h"
namespace NUISystem {
CTextContainer::CTextContainer(long def)
    : CChangingStateComponent(def)
{
    GetDataFromDef();
}
}