// NUISystem::CTable::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18,
//   jump to the deleting destructor 0x005540D0
// Leaf: ??_ECTable@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00553FE0, 8 bytes: `sub ecx,18h ; jmp 0x005540D0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CTable's CActionParam vtable 0x0124A1F4 at +0x18 (rtti-vtables.tsv, RTTI .?AVCTable@NUISystem@@),
//   and its jump target 0x005540D0 is slot 0 of the primary vtable 0x0124A224, the class's ??_G (calls ~CTable 0x00550D50); VC7.1
//   spells the +0x18 thunk ??_E...W BI@ (adjustor 24), as objdump -t of this object shows. Twin of the matched +4 thunk 0x00553FD0
//   (??_ECTable@NUISystem@@W3AEPAXI@Z).
// Was: the placeholder CMulti_005540d0::Override_005540d0 adjustor (local CFirstBase / CSecondBase / CMulti_005540d0 model,
//   rebuild/src/compiled/00/55/unattributed_Override005540d0_00553fe0.cpp).
// Callee: ??_ECTable@NUISystem@@UAEPAXI@Z 0x005540D0 (the weak ??_E alias of ??_GCTable@NUISystem@@UAEPAXI@Z).
// TU: as 0x00553FD0: the constructor's TU, CTable(long) (0x005517E0, its own landed row, not graded here); only the thunk is this row.
// Header: engine/NUISystem_CTable.h.
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
