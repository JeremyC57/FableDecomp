// NUISystem::CMenuEntry::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x0053E8E0
// Leaf: ??_ECMenuEntry@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053E8C0, 8 bytes: `sub ecx,4 ; jmp 0x0053E8E0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CMenuEntry's CObserver vtable 0x01246604 at +4 (rtti-vtables.tsv, RTTI .?AVCMenuEntry@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0053E8E0 is slot 0 of the primary vtable 0x0124662C, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CMenuEntry(long) (0x0053DFE0: CClickable(long) 0x0055B460, the vptrs, IsAvailable and the eight action-list pointers, then GetDataFromDef 0x0053DA30).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCMenuEntry@NUISystem@@UAEPAXI@Z 0x0053E8E0 (bound as its ??_E alias).
// Header: engine/NUISystem_CMenuEntry.h.
// Twin: 0x0053E8D0 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CMenuEntry.h"
namespace NUISystem {
CMenuEntry::CMenuEntry(long def)
    : CClickable(def)
    , IsAvailable(true)
    , PUIActionParams(0)
    , PThingActionParams(0)
    , PActions(0)
    , PActionsOnSelected(0)
    , PActionsOnBack(0)
    , PPreActions(0)
    , PActionsOnUnselected(0)
    , PActionsOnVisible(0)
{
    GetDataFromDef();
}
}