// NUISystem::CClickable::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x0055B850
// Leaf: ??_ECClickable@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055B830, 8 bytes: `sub ecx,4 ; jmp 0x0055B850`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CClickable's CObserver vtable 0x0124BD08 at +4 (rtti-vtables.tsv, RTTI .?AVCClickable@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0055B850 is slot 0 of the primary vtable 0x0124BD2C, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CClickable (0x0055B760: delete the four owned action lists, then ~CHoverable).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCClickable@NUISystem@@UAEPAXI@Z 0x0055B850 (bound as its ??_E alias).
// Header: engine/NUISystem_CClickable.h.
// Twin: 0x0055B840 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
CClickable::~CClickable()
{
    delete PActionsOnLeftClicked;
    delete PActionsOnRightClicked;
    delete PActionsOnLeftHeld;
    delete PActionsOnLeftUnclicked;
}
}