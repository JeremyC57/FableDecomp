// NUISystem::CScrollingComponent::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00547260
// Leaf: ??_ECScrollingComponent@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547240, 8 bytes: `sub ecx,4 ; jmp 0x00547260`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CScrollingComponent's CObserver vtable 0x012482A8 at +4 (rtti-vtables.tsv, RTTI .?AVCScrollingComponent@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00547260 is slot 0 of the primary vtable 0x012482CC, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CScrollingComponent(long) (0x00546F40: CChangingStateComponent(long) 0x0052CC50 and the three vptrs).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCScrollingComponent@NUISystem@@UAEPAXI@Z 0x00547260 (bound as its ??_E alias).
// Header: engine/NUISystem_CScrollingComponent.h.
// Twin: 0x00547250 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CScrollingComponent.h"
namespace NUISystem {
CScrollingComponent::CScrollingComponent(long def)
    : CChangingStateComponent(def)
{
}
}