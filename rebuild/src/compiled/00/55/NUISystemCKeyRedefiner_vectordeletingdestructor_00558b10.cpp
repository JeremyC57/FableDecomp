// NUISystem::CKeyRedefiner::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x00558B30
// Leaf: ??_ECKeyRedefiner@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558B10, 8 bytes: `sub ecx,4 ; jmp 0x00558B30`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CKeyRedefiner's CObserver vtable 0x0124AD98 at +4 (rtti-vtables.tsv, RTTI .?AVCKeyRedefiner@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x00558B30 is slot 0 of the primary vtable 0x0124ADBC, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CKeyRedefiner (0x00558660, landed: clear the selected-redefiner global; the compiler destroys the Actions list and calls ~CClickable).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCKeyRedefiner@NUISystem@@UAEPAXI@Z 0x00558B30 (bound as its ??_E alias).
// Header: engine/NUISystem_CKeyRedefiner.h.
// Twin: 0x00558B20 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CKeyRedefiner.h"
namespace NUISystem {
CKeyRedefiner::~CKeyRedefiner()
{
    if (g_pSelectedKeyRedefiner == this)
        g_pSelectedKeyRedefiner = 0;
}
}