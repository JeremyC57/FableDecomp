// NUISystem::CTable::`scalar deleting destructor'(unsigned int flags) -- run ~CTable, free the control when bit 0 is set
// Leaf: ??_GCTable@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005540D0, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CTable's primary vtable 0x0124A224 (rtti-vtables.tsv, RTTI .?AVCTable@NUISystem@@); it calls
//   ~CTable 0x00550D50 (FableWin.pdb ??1CTable@NUISystem@@UAE@XZ) and operator delete 0x00BFE9BC; the adjustor thunks 0x00553FD0 (+4)
//   and 0x00553FE0 (+0x18) jump to it. VC7.1 emits the function as ??_G and the vtable / thunk entry ??_ECTable@NUISystem@@UAEPAXI@Z
//   (FableWin.pdb's slot-0 name) as a weak alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the free stand-in CTable_scalar_deleting_destructor (struct CTable model, rebuild/src/compiled/00/55/
//   NUISystemCTable_scalardeletingdestructor_005540d0.cpp).
// TU: as the matched +4 thunk 0x00553FD0: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this
//   source is the constructor's TU, CTable(long) (0x005517E0, its own landed row, not graded here); ~CTable stays declared only, so the
//   ??_G calls it out of line, as retail does. Only the ??_G is this row.
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
