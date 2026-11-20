// NUISystem::CClickable::`scalar deleting destructor'(unsigned int flags) -- run ~CClickable, free the control when bit 0 is set
// Leaf: ??_GCClickable@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055B850, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CClickable's primary vtable 0x0124BD2C (RTTI .?AVCClickable@NUISystem@@; its only data reference);
//   it calls ~CClickable 0x0055B760 (Ego_r.pdb / FableWin.pdb ??1CClickable@NUISystem@@UAE@XZ, the landed self row) and operator delete
//   0x00BFE9BC; the matched adjustor thunks 0x0055B830 (+4) / 0x0055B840 (+0x18) jump to it. VC7.1 emits the function as ??_G and the
//   vtable / thunk entry ??_ECClickable@NUISystem@@UAEPAXI@Z as a weak alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the stand-in CClickable::ScalarDeletingDestructor calling CClickable::Dtor (?Dtor@CClickable@@QAEXXZ, local struct model,
//   rebuild/src/compiled/00/55/NUISystemCClickable_scalardeletingdestructor_0055b850.cpp).
// TU: the destructor's TU, as the matched thunk 0x0055B830 (~CClickable 0x0055B760 defined here makes VC7.1 emit ??_G and both thunks; it
//   is its own row, not graded here, and stays a call in the ??_G, as in retail). Only the ??_G is this row.
// Header: engine/NUISystem_CClickable.h.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
CClickable::~CClickable()
{
    delete PActionsOnLeftClicked;
    delete PActionsOnRightClicked;
    delete PActionsOnLeftHeld;
    delete PActionsOnLeftUnclicked;
}
}