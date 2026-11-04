// NUISystem::CTable::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CObserver-subobject (+4) vtable entry: this -= 4, jump to the deleting destructor 0x005540D0
// Leaf: ??_ECTable@NUISystem@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00553FD0, 8 bytes: `sub ecx,4 ; jmp 0x005540D0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CTable's CObserver vtable 0x0124A1FC at +4 (rtti-vtables.tsv, RTTI .?AVCTable@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x005540D0 is slot 0 of the primary vtable 0x0124A224, the class's ??_G (it calls
//   ~CTable 0x00550D50; FableWin.pdb ??_ECTable@NUISystem@@UAEPAXI@Z). VC7.1 names the slot-0 entry ??_E (a weak alias of ??_G
//   when the TU has no array new) and spells the thunk with the adjustor (W3 = 4); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU:
//   CTable(long) (0x005517E0: CChangingStateComponent(long) 0x0052CC50, the vptrs, the empty Sprites map, no separation vectors,
//   GetDataFromDef 0x00551340). That function is not graded here (it is its own landed row); only the thunk is this row.
// Callees: ??_GCTable@NUISystem@@UAEPAXI@Z 0x005540D0 (bound as its ??_E alias; landed placeholder ?Override_005540d0@CMulti_005540d0@@UAEXXZ, NAME_FIXES.tsv).
// Header: engine/NUISystem_CTable.h (new).
// Twin: 0x00553FE0 is the +0x18 (CActionParam) adjustor thunk of the same vtable set.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/NUISystem_CTable.h"
namespace NUISystem {
CTable::CTable(long owner)
    : CChangingStateComponent(owner)
    , PHorizontalSeparations(0)
    , PVerticalSeparations(0)
{
    GetDataFromDef();
}
}