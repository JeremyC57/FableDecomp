// NUISystem::CMenuEntry::`scalar deleting destructor'(unsigned int flags) -- run ~CMenuEntry, free the entry when bit 0 is set
// Leaf: ??_GCMenuEntry@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053E8E0, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CMenuEntry's primary vtable 0x0124662C (RTTI .?AVCMenuEntry@NUISystem@@); it calls ~CMenuEntry
//   0x0053E4E0 (Ego_r.pdb / FableWin.pdb ??1CMenuEntry@NUISystem@@UAE@XZ; its only caller) and operator delete 0x00BFE9BC; the matched
//   adjustor thunks 0x0053E8C0 / 0x0053E8D0 jump to it. VC7.1 emits the function as ??_G and the vtable / thunk entry
//   ??_ECMenuEntry@NUISystem@@UAEPAXI@Z as a weak alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the stand-in CMenuEntry::ScalarDeletingDestructor calling CMenuEntry::Dtor (?Dtor@CMenuEntry@@QAEXXZ, local struct model,
//   rebuild/src/compiled/00/53/NUISystemCMenuEntry_scalardeletingdestructor_0053e8e0.cpp).
// TU: the constructor's TU, as the matched thunk 0x0053E8C0 (CMenuEntry(long) 0x0053DFE0, its own row, not graded here); ~CMenuEntry is
//   declared only, so the ??_G calls it out of line, as retail does. Only the ??_G is this row.
// Header: engine/NUISystem_CMenuEntry.h.
#include "engine/NUISystem_CMenuEntry.h"
namespace NUISystem {
CMenuEntry::CMenuEntry(long def)
    : CClickable(def)
    , IsAvailable(true)
    , PUIActionParams(0)
    , PThingActionParams(0)
    , PActions(0)
    , PActionsOnSelected(0)
    , PActionsOnBack(0)
    , PPreActions(0)
    , PActionsOnUnselected(0)
    , PActionsOnVisible(0)
{
    GetDataFromDef();
}
}