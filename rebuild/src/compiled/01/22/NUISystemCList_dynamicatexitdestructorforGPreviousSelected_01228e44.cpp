// NUISystem::CList::`dynamic atexit destructor for 'GPreviousSelected'' (0x013B8A98) -- at exit run the out-of-line std::list<NUISystem::CList*> destructor (0x0053C9BC) on NUISystem::CList::GPreviousSelected
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01228E44, 10 bytes, `.text$yd`: `mov ecx,0x013B8A98 ; jmp 0x0053C9BC`.
// agentNF187 (q4154): the landed source was an invented AutoTinyThunkTarget stand-in (GlobalMethodTailThunk_01228e44) whose Leaf
//   named no symbol. Identity, two sources: Ego_r.pdb 0x0101BC4A and FableWin.pdb 0x0400F312 name `dynamic atexit destructor for
//   'NUISystem::CList::GPreviousSelected'' (each beside the `dynamic initializer' at 0x0100DFCD / 0x03FE51B1); the only reference to
//   0x01228E44 is the initializer 0x0121A6AC's `mov [esp],0x01228E44 ; call _atexit`. The compiler emits this function from the
//   definition of the static on the shared header (as the nf89 / nf164 pairs 0x0121F0E0 / 0x01229BE0 and 0x0121E030 / 0x01229280);
//   it jumps to ~list<NUISystem::CList*> (FableWin.pdb ??1?$list@PAVCList@NUISystem@@...@std@@QAE@XZ), declared only (FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY): the retail
//   TU calls the out-of-line copy and this row emits none.
// Pragma: `#pragma optimize("s", on)` -- the retail CList TU is size-optimised (the initializer 0x0121A6AC and ~list 0x0053C9BC).
#pragma optimize("s", on)
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY
#include "engine/NUISystem_CList.h"

std::list<NUISystem::CList*> NUISystem::CList::GPreviousSelected;   // 0x013B8A98
