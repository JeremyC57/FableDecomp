// NUISystem::CChangingStateComponent::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x0052D770
// Leaf: ??_ECChangingStateComponent@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052D700, 8 bytes: `sub ecx,4 ; jmp 0x0052D770`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CChangingStateComponent's CObserver vtable 0x01245DBC at +4 (rtti-vtables.tsv, RTTI .?AVCChangingStateComponent@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0052D770 is slot 0 of the primary vtable 0x01245DE4, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CChangingStateComponent (0x0052CCF0) has an empty user body (the compiler stores the vptrs, destroys the StatesToDo list and calls ~CComponent).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCChangingStateComponent@NUISystem@@UAEPAXI@Z 0x0052D770 (bound as its ??_E alias).
// Header: engine/NUISystem_CChangingStateComponent.h.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
CChangingStateComponent::~CChangingStateComponent()
{
}
}