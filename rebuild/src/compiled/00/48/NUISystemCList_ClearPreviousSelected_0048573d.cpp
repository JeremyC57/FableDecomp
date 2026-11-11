// NUISystem::CList::ClearPreviousSelected() -- empty the static list of previously selected lists (GPreviousSelected)
// Leaf: ?ClearPreviousSelected@CList@NUISystem@@SIXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048573D, 10 bytes: `mov ecx, offset 0x013B8A98 ; jmp 0x004855A3`.
// Identity (proven, agentFAM07): FableWin.pdb ?ClearPreviousSelected@CList@NUISystem@@SIXXZ (public static __fastcall void(),
//   also in the Ego_r name set); its global 0x013B8A98 is NUISystem::CList::GPreviousSelected (Ego_r `dynamic initializer for
//   'NUISystem::CList::GPreviousSelected'`, landed spelling in engine/NUISystem_CList.h) and the jump target 0x004855A3 is that
//   list's out-of-line STLport _List_base<CList*>::clear(). Only caller: CTCQuestCompletionUI::GlobalUpdate 0x005E78F0.
//   Was the placeholder CUnprovenForwarder_0048573d.
// Callees: 0x004855A3 ?clear@?$_List_base@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAEXXZ (STLport 4.6
//   _list.c, declared only: FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE).
// Headers: engine/NUISystem_CList.h opt-in FABLE_RETAIL_CLIST_CLEARPREVIOUSSELECTED (declares the static member).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CLIST_CLEARPREVIOUSSELECTED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#include "engine/NUISystem_CList.h"

void __fastcall NUISystem::CList::ClearPreviousSelected()
{
    GPreviousSelected.clear();
}
