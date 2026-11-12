// std::_S_sort<const CThingCreatureBase*, allocator<const CThingCreatureBase*>, CThingCompare_Nearest>(list& __that, CThingCompare_Nearest __comp)
//   -- STLport 4.6 _list.c list merge sort, the instance behind list<const CThingCreatureBase*>::sort(CThingCompare_Nearest)
//   (CCombatManager's creature lists). Retail quirk: this does NOT order by distance. CThingCompare_Nearest::operator()
//   (0x006EF290) returns the nearer POINTER, a T* (`mov eax,edi / je / mov eax,ebx / ret 8`), and _S_merge only tests that
//   result for non-null (`test eax,eax`, 0x006F04E1..0x006F04E8), so any two non-null creatures compare true and the sort just
//   reshuffles the list by the merge pattern; no nearest-first ordering happens.
// Leaf: ??$_S_sort@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@VCThingCompare_Nearest@@@std@@YIXAAV?$list@PBVCThingCreatureBase@@V?$allocator@PBVCThingCreatureBase@@@std@@@0@VCThingCompare_Nearest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F3A30, 481 bytes, `ret 0Ch` (the 12-byte comparator by value on the stack,
//   the list in ecx: the STL TU is /Gr, so the free template is __fastcall -- FABLE_STLP_FASTCALL in the shared header).
// Identity: the member list::sort(_StrictWeakOrdering) is the 0x31-byte forwarder at 0x006F4040 (`_S_sort(*this, __comp);`);
//   element type and comparator from ??$sort@VCThingCompare_Nearest@@@?$list@PBVCThingCreatureBase@@...@std@@QAEXVCThingCompare_Nearest@@@Z
//   (Ego_r.pdb 0x0071A120, FableWin.pdb 0x02433550) and the callers (CCombatManager list rebuild 0x006EBA60).
// Callees: malloc 0x00BFEA0E (carry's header node, list constructor inline), `vector constructor iterator` 0x004038C0 with the
//   list default-constructor closure 0x006F2BB0 (the 64 counters), _S_merge<...> 0x006F04C0 (out of line, twice), free 0x00BFEA14
//   (the inline list destructors: counters 63..0, then carry).
// Shape: a list of 0 or 1 nodes returns at once; otherwise the first node is spliced into `carry` (list::splice ->
//   _List_global::_Transfer inline), merged up the counter ladder (`_S_merge(counter[i], carry); carry.swap(counter[i++])`), the
//   ladder merged down into counter[fill-1] and swapped back into the list. What makes the bytes: the _S_merge BODY is visible
//   to the compiler (STLport's _list.c is included by the same TU) though called out of line, so c2 knows the call does not
//   write carry's head word: carry._M_node._M_data lives in ebx across the calls (stored back to [esp+14h] only for the
//   by-reference argument), its `= 0` constructor store is dead, and its final deallocate needs no null test. With _S_merge
//   declared only, the head is reloaded from the frame and the body is 496 bytes. The comparator's operator() is declared only
//   (FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE): retail calls its instance 0x006EF290 out of line from _S_merge.
#define FABLE_RETAIL_STLP_LIST_SORT
#define FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CThingCompare_Nearest.h"
class CThingCreatureBase;
template void FABLE_STLP_FASTCALL std::_S_sort<const CThingCreatureBase*,std::allocator<const CThingCreatureBase*>,CThingCompare_Nearest>(std::list<const CThingCreatureBase*,std::allocator<const CThingCreatureBase*> >&,CThingCompare_Nearest);