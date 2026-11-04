// NUISystem::CListArrow::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x0054C360
// Leaf: ??_ECListArrow@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054C340, 8 bytes: `sub ecx,4 ; jmp 0x0054C360`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CListArrow's CObserver vtable 0x01248F6C at +4 (rtti-vtables.tsv, RTTI .?AVCListArrow@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x0054C360 is slot 0 of the primary vtable 0x01248F94, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CListArrow(long) (0x0054C1D0: CSprite(long) 0x0041B800 and the three vptrs; no own members).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCListArrow@NUISystem@@UAEPAXI@Z 0x0054C360 (bound as its ??_E alias).
// Header: engine/NUISystem_CListArrow.h.
// Twin: 0x0054C350 is the other adjustor thunk of the same vtable set (same TU).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/NUISystem_CListArrow.h"
namespace NUISystem {
CListArrow::CListArrow(long def)
    : CSprite(def)
{
}
}