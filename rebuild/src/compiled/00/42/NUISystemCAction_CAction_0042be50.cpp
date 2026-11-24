// NUISystem::CAction::CAction() -- ActionType = 0, the two empty STLport lists (UIActionParams, ThingActionParams), Valid = true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042BE50, length 43 bytes.
// Leaf: ??0CAction@NUISystem@@QAE@XZ
// Retail bytes: push ebp ; mov ebp,esp ; push ecx ; push esi ; mov esi,ecx ; and dword ptr [esi],0 ; lea eax,[ebp-1] ; push eax ; lea ecx,[esi+4] ; call list ;
//               lea eax,[ebp-1] ; push eax ; lea ecx,[esi+8] ; call list ; mov byte ptr [esi+0Ch],1 ; mov eax,esi ; pop esi ; leave ; ret
// Callees: the STLport list constructors (allocator by const reference; `[this]=0 ; malloc(0Ch) ; next=prev=node`)
// @ 0x0042AB01 (list<CActionParam*>) and @ 0x0042AB58 (_List_base<long, allocator<long> >, Codex name fix 2026-11-10).
// The manifest label "CRumbleDef::CRumbleDef" is propagated (CRumbleDef is 72 bytes and has no lists at +4/+8): the
// callers (NUISystem::CManager::AddActionsListenIgnoreOnList 0x0041DC9D and friends) allocate 0x10 bytes, run this
// constructor and hand the object to CCountedPointer<NUISystem::CAction> (0x0042AA29). PDB layout of CAction: ActionType +0,
// UIActionParams +4, ThingActionParams (+16 in the PDB's 12-byte debug lists; +8 with retail's 4-byte lists), Valid.
// Shape: a frame-pointer, size-optimised translation unit (`optimize("y",off)` + `("s",on)`, as the neighbouring
// landed NUISystem::CObservable constructor 0x0042BE7B): `and [esi],0` for the zero, the default allocator argument of
// each list a 1-byte empty temporary homed at [ebp-1] (both lists reuse the slot).
// Port (NF227, 2026-11-22, q12578 / q12584): onto engine/NUISystem_CAction.h (FableWin.pdb ??0CAction@NUISystem@@QAE@XZ; members
//   ActionType +0, list<CActionParam*> UIActionParams +4, list<long> ThingActionParams +8, Valid +0x0C). Was a row-local CAction with a
//   CActionParamList / SRetailAllocator stand-in for UIActionParams, whose constructor bound 0x0042AB01 as
//   ??0CActionParamList@NUISystem@@QAE@ABUSRetailAllocator@1@@Z. Both lists now use the header's STLport list with
//   FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (the inline list(const allocator&) calls the out-of-line _List_base constructor), so the call
//   binds ??0?$_List_base@PAVCActionParam@NUISystem@@V?$allocator@PAVCActionParam@NUISystem@@@std@@@std@@QAE@ABV?$allocator@PAVCActionParam@NUISystem@@@1@@Z
//   at 0x0042AB01, the spelling 0x0041E3F6 binds there (retail folds the list ctor into it).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "engine/NUISystem_CAction.h"
// The TU is frame-pointer and size-optimised; the pragmas follow the includes (EXTRA_RULES 2026-11-20, nf189 placement).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem {
CAction::CAction()
    : ActionType(UI_ACTION_TYPE_NONE), Valid(true)
{
}
}
