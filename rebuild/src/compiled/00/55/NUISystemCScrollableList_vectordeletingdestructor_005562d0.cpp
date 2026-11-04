// NUISystem::CScrollableList::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x005562F0
// Leaf: ??_ECScrollableList@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005562D0, 8 bytes: `sub ecx,4 ; jmp 0x005562F0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CScrollableList's CObserver vtable 0x0124A708 at +4 (rtti-vtables.tsv, RTTI .?AVCScrollableList@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x005562F0 is slot 0 of the primary vtable 0x0124A72C, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CScrollableList(long) (0x00555180: CChangingStateComponent(long) 0x0052CC50, ScrollTimer, both arrow pointers and both arrow flags cleared, the vptrs).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCScrollableList@NUISystem@@UAEPAXI@Z 0x005562F0 (bound as its ??_E alias).
// Header: engine/NUISystem_CScrollableList.h.
// Twin: 0x005562E0 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CScrollableList.h"
namespace NUISystem {
CScrollableList::CScrollableList(long def)
    : CChangingStateComponent(def)
    , ScrollTimer(0.0f)
    , PUpArrow(0)
    , PDownArrow(0)
    , UpArrowActive(false)
    , DownArrowActive(false)
{
}
}