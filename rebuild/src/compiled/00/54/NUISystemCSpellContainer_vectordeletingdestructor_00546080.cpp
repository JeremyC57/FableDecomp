// NUISystem::CSpellContainer::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x005460A0
// Leaf: ??_ECSpellContainer@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546080, 8 bytes: `sub ecx,4 ; jmp 0x005460A0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CSpellContainer's CObserver vtable 0x01247848 at +4 (rtti-vtables.tsv, RTTI .?AVCSpellContainer@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x005460A0 is slot 0 of the primary vtable 0x0124786C, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the destructor's TU: ~CSpellContainer (0x00545840: remove the observer from CManager; the compiler destroys BaseSpells and the base).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCSpellContainer@NUISystem@@UAEPAXI@Z 0x005460A0 (bound as its ??_E alias).
// Header: engine/NUISystem_CSpellContainer.h.
// Twin: 0x00546090 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CSpellContainer.h"
namespace NUISystem {
CSpellContainer::~CSpellContainer()
{
    CManager::GetInstance()->RemoveObserver(this);
}
}