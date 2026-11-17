// std::__uninitialized_copy(map* first, map* last, map* result, const __false_type&) -- STLport placement-copy of [first, last) to result for the vector<map<unsigned long, NUISystem::CUIState> > of NUISystem::CList::ScrollingChildrenStates: copy-construct each 12-byte map through the out-of-line map copy constructor (null-tested placement), return the advanced destination
// Leaf: ??$__uninitialized_copy@PAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@PAV12@@std@@YIPAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@0@PAV10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CCA5, 43 bytes, __fastcall (first ecx, last edx, result and the tag pointer pushed: `ret 8`),
//   size-optimised TU without a frame pointer.
// Retail bytes:  push ebx ; push esi ; mov ebx,edx ; mov esi,ecx ; cmp esi,ebx ; push edi ; mov edi,[esp+10h] ; je E ; L: test edi,edi ;
//   je S ; push esi ; mov ecx,edi ; call map::map(const map&) 0x0053CB9B ; S: add esi,12 ; add edi,12 ; cmp esi,ebx ; jne L ;
//   E: mov eax,edi ; pop edi ; pop esi ; pop ebx ; ret 8
// Callees: map<unsigned long, NUISystem::CUIState>::map(const map&) @ 0x0053CB9B (FableWin.pdb 0x021A7386
//   ??0?$map@KVCUIState@NUISystem@@...@std@@QAE@ABV01@@Z; STLport _Construct(map*, const map&), FableWin.pdb 0x021A7283).
// Callers: vector<map<unsigned long, NUISystem::CUIState> >::_M_insert_overflow 0x0053CEF6 (landed: head and tail copies; its /Gr
//   explicit instantiation __uninitialized_copy<map*, map*> carries this spelling).
// Name fix (NF59, queue q10531; NF56 porting 0x0053CFC2): the landed row was the generated `UninitCopy_CElement12_0053cb9b_0053cca5` over
//   a 12-byte stand-in element; it is STLport 4.6's non-POD __uninitialized_copy (stlport/fable_containers.h FABLE_STLP_OUTOFLINE_UNINIT_COPY:
//   the same-TU noinline __fastcall body) over the shared map (stlport/fable_map.h, FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE: the copy
//   constructor declared, as 0x0053CFC2 declares it) and engine/NUISystem_CUIState.h. No PDB records this instance (inline in Ego_r);
//   the spelling is the one the shared header emits, matching the caller 0x0053CEF6's instantiation.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIState.h"

typedef std::map<unsigned long, NUISystem::CUIState> CUIStateMap;
template CUIStateMap* __fastcall std::__uninitialized_copy(CUIStateMap* first, CUIStateMap* last, CUIStateMap* result, const std::__false_type&);
