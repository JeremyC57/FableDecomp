// [thunk] NUISystem::CFrontEndList::`vector deleting destructor' `adjustor{4}'(unsigned int) -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x0054DA70
// Leaf: ??_ECFrontEndList@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054DA50, 8 bytes: `sub ecx,4 ; jmp 0x0054DA70`. Slot 0 of CFrontEndList's CObserver vtable
//   0x012491FC (rtti-vtables.tsv; the catalog's "vfunc_0"). Compiler-generated adjustor thunk, ??_E alias of ??_G 0x0054DA70 (landed).
//
// TU: the CFrontEndList constructor translation unit (landed body 0x0054C3A0: CList(long) then the three vptrs). The empty inline
// destructor means the vtables, the ??_G 0x0054DA70 and both adjustor thunks are emitted where the vtable is first needed: this
// constructor. Retail places the thunks right before the ??_G, after the class's code.
// Header: engine/NUISystem_CFrontEndList.h (new, chunk 753) over engine/NUISystem_CList.h (FABLE_RETAIL_CLIST: retail sizeof 0x1FC).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CLIST
#include "engine/NUISystem_CFrontEndList.h"
namespace NUISystem {
CFrontEndList::CFrontEndList(long defIndex)
    : CList(defIndex)
{
}
}