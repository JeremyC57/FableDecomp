// NUISystem::CHoverable::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x0055C4A0
// Leaf: ??_ECHoverable@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C1F0, 8 bytes: `sub ecx,4 ; jmp 0x0055C4A0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CHoverable's CObserver vtable 0x0124BF90 at +4 (rtti-vtables.tsv, RTTI .?AVCHoverable@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0055C4A0 is slot 0 of the primary vtable 0x0124BFB4, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CHoverable(long) (0x0055BA20: CChangingStateComponent(long) 0x0052CC50, the vptrs, the hover state cleared, then the CObserver sub-object registered with CManager, slot 2 AddObserver).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCHoverable@NUISystem@@UAEPAXI@Z 0x0055C4A0 (bound as its ??_E alias).
// Header: engine/NUISystem_CClickable.h.
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
CHoverable::CHoverable(long def)
    : CChangingStateComponent(def)
    , StateBeforeHovered(0)
    , WasHovered(false)
    , PActionsOnHovered(0)
    , PActionsOnUnhovered(0)
{
    CManager::GetInstance()->AddObserver(this);
}
}