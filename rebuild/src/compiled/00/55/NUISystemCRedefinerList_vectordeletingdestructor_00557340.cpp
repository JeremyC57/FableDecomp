// NUISystem::CRedefinerList::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00557360
// Leaf: ??_ECRedefinerList@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00557340, 8 bytes: `sub ecx,4 ; jmp 0x00557360`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CRedefinerList's CObserver vtable 0x0124A970 at +4 (rtti-vtables.tsv, RTTI .?AVCRedefinerList@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00557360 is slot 0 of the primary vtable 0x0124A994, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CRedefinerList(long) (0x00556350: CScrollableList(long) 0x00555180, the vptrs, the activation / fade / warning state cleared).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCRedefinerList@NUISystem@@UAEPAXI@Z 0x00557360 (bound as its ??_E alias).
// Header: engine/NUISystem_CRedefinerList.h.
// Twin: 0x00557350 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CRedefinerList.h"
namespace NUISystem {
CRedefinerList::CRedefinerList(long def)
    : CScrollableList(def)
    , IsActivated(false)
    , RefreshAfterFade(false)
    , FadeOutTime(0.0f)
    , HaveUndefinedControls(false)
    , PUndefinedWarning(0)
    , PAcceptButton(0)
{
}
}