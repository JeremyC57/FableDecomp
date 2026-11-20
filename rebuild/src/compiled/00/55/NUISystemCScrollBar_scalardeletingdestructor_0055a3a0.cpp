// NUISystem::CScrollBar::`scalar deleting destructor'(unsigned int flags) -- run ~CScrollBar, free the control when bit 0 is set
// Leaf: ??_GCScrollBar@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055A3A0, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CScrollBar's primary vtable 0x0124B7E4 (RTTI .?AVCScrollBar@NUISystem@@); it calls ~CScrollBar
//   0x00559A10 and operator delete 0x00BFE9BC; the matched adjustor thunks 0x0055A380 (+4) / 0x0055A390 (+0x18) jump to it. VC7.1 emits
//   the function as ??_G and the vtable / thunk entry ??_ECScrollBar@NUISystem@@UAEPAXI@Z (Ego_r.pdb's slot-0 name, 0x005D3730) as a weak
//   alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the stand-in CScrollBar::ScalarDeletingDestructor (?ScalarDeletingDestructor@CScrollBar@@QAEPAXI@Z, local struct model,
//   rebuild/src/compiled/00/55/NUISystemCScrollBar_scalardeletingdestructor_0055a3a0.cpp).
// TU: the destructor's TU, as the matched thunk 0x0055A380 (~CScrollBar 0x00559A10 defined here makes VC7.1 emit ??_G and both thunks;
//   it is its own row, not graded here, and stays a call in the ??_G, as in retail). Only the ??_G is this row.
// Header: engine/NUISystem_CScrollBar.h.
#include "engine/NUISystem_CScrollBar.h"
namespace NUISystem {
CScrollBar::~CScrollBar()
{
    delete PActionsOnDraggedDown;
    delete PActionsOnDraggedUp;
}
}
