// NUISystem::CComponentContainer::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x005465D0
// Leaf: ??_ECComponentContainer@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005465B0, 8 bytes: `sub ecx,4 ; jmp 0x005465D0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CComponentContainer's CObserver vtable 0x01247ACC at +4 (rtti-vtables.tsv, RTTI .?AVCComponentContainer@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x005465D0 is slot 0 of the primary vtable 0x01247AF4, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CComponentContainer(long) (0x005460C0: CChangingStateComponent(long) 0x0052CC50 and the three vptrs; the class adds no members).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCComponentContainer@NUISystem@@UAEPAXI@Z 0x005465D0 (bound as its ??_E alias).
// Header: engine/NUISystem_CComponentContainer.h.
// Twin: 0x005465C0 is the other adjustor thunk of the same vtable set (same TU).
#define FABLE_RETAIL_CCOMPONENTCONTAINER
#define FABLE_RETAIL_CCOMPONENTCONTAINER_CTORS
#include "engine/NUISystem_CComponentContainer.h"
namespace NUISystem {
CComponentContainer::CComponentContainer(long def)
    : CChangingStateComponent(def)
{
}
}