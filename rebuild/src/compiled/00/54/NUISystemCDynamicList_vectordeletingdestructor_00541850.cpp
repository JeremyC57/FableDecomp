// NUISystem::CDynamicList::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00541870
// Leaf: ??_ECDynamicList@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00541850, 8 bytes: `sub ecx,4 ; jmp 0x00541870`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CDynamicList's CObserver vtable 0x01246E20 at +4 (rtti-vtables.tsv, RTTI .?AVCDynamicList@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00541870 is slot 0 of the primary vtable 0x01246E44, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CDynamicList (0x00541350) has an empty user body (vptrs, the ScrollingChildren vector's storage, ~CChangingStateComponent).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCDynamicList@NUISystem@@UAEPAXI@Z 0x00541870 (bound as its ??_E alias).
// Header: engine/NUISystem_CDynamicList.h.
// Twin: 0x00541860 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CDynamicList.h"
namespace NUISystem {
CDynamicList::~CDynamicList()
{
}
}