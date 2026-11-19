// NUISystem::CList::AddActionOnSelected(CCountedPointer<NUISystem::CAction>& action) -- create the list's PActionsOnSelected action list on first use, then append the action to it
// Leaf: ?AddActionOnSelected@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe @ 0x005384DD, 78 bytes, virtual thiscall `ret 4`; size-optimised frame-pointer TU (ui_list.obj).
// Identity: Ego_r.pdb ui_list.obj #29 / FableWin.pdb #57 (AddActionOnInactive, AddActionOnSelected in that order, retail 0x0053848F /
//   0x005384DD back to back); the list is CList::PActionsOnSelected (retail +0x1B0, engine/NUISystem_CList.h); the catalog's AddActionOnInactive label on this address was a byte-shape propagation (NAME_FIXES.tsv).
// Callees: operator new 0x00BFEA1A; STLport _List_base(const allocator&) 0x00535140 (list's inline default constructor calls it out of
//   line; retail keeps the new object in edi and ignores the base constructor's return); list::push_back 0x00535710.
// Shape: FableWin /Od 0x0219D2EC: `if (!PActionsOnSelected) PActionsOnSelected = new list; PActionsOnSelected->push_back(action);` (the new-expression
//   leaves the state store `and [ebp-4],0`). The ui_list TU pragmas are those of the landed same-TU rows (0x005383DD, 0x0053852B).
//   Helper bodies: the shared fable_containers.h _List_base constructor (FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE) and push_back
//   (FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE) both grade RELOCATION_MATCH at their own retail copies 0x00535140 / 0x00535710 under /O2.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE
#include "engine/NUISystem_CList.h"

void NUISystem::CList::AddActionOnSelected(CCountedPointer<NUISystem::CAction>& action)
{
    if (!PActionsOnSelected)
        PActionsOnSelected = new std::list<CCountedPointer<NUISystem::CAction> >;
    PActionsOnSelected->push_back(action);
}
