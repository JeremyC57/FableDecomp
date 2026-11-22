// std::__push_heap<pair<unsigned __int64, list<C2DLineF> >*, int, pair<unsigned __int64, list<C2DLineF> >,
// CKeyPairCompareLess<unsigned __int64, list<C2DLineF> > >(first, holeIndex, topIndex, val, comp) -- percolate a
// CNavigationLayer::SwitchableLineLists entry up a max-heap, then destroy the by-value list
// Flags: /Gr
// Retail: Fable.exe @ 0x00A820E0, 152 bytes (/O2). Called from __adjust_heap @ 0x00A82BE0. Callees: list<C2DLineF>::operator=
// 0x00517720 (catalog "std::list::operator=") and the list base destructor _List_base<C2DLineF>::~_List_base 0x005127A0 (walks and
// frees the nodes, then the sentinel).
// Shape: STLport 4.6 `__push_heap` (`holeIndex > topIndex` test, rotated loop) over a 16-byte element (`shl eax,4`): the
// UNSIGNED 64-bit key +0 (high word compared first: `ja done ; jb move`, then the low word `jae done`) and the one-word
// STLport list<C2DLineF> +8 (+0xC padding). The implicit pair operator= is the two key words then the list's out-of-line
// operator=; the by-value pair is destroyed at the end (list's inline `~list() {}`, the out-of-line ~_List_base).
// /Gr: first=ecx, holeIndex=edx; `ret 18h` = topIndex + the 16-byte pair + the empty comparator.
// Name: Ego_r.pdb 0x00BC46D0 `??$_Push_heap@PAU?$pair@_KV?$list@VC2DLineF@@...@std@@HU12@V?$CKeyPairCompareLess@_KV?$list@
//   VC2DLineF@@...@@@std@@YIX...` is the same instantiation under Dinkumware (element pair<unsigned __int64,
//   list<C2DLineF> >, the CNavigationLayer::SwitchableLineLists value_type); retail's STLport spelling is __push_heap.
//   Was catalogued `std::_Push_heap` over a descriptive 8-byte SList8_00517720 (retail STLport lists are one word).
// Leaf: ??$__push_heap@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@HU12@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@std@@YIXPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@HHU10@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@Z
// NF246 (2026-11-22, the 0x005127A0 family, nf243 NOTES section 2): the row-local C2DVector / C2DLineF / allocator / list / pair /
//   CKeyPairCompareLess models and the local __push_heap body replaced by the shared ones (engine/C2DLineF.h, engine/CVectorMap.h,
//   stlport/fable_containers.h FABLE_RETAIL_STLP_DECAL_HEAP), with the STLport list as this TU calls it, the macro set of its landed
//   caller __adjust_heap 0x00A82BE0 (whose pick-any __push_heap copy is this body, EQ 0x00A820E0): operator= out of line (0x00517720,
//   FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL), _M_insert_dispatch out of line, ~_List_base out of line (0x005127A0,
//   FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE): the by-value list's destructor call binds ??1?$_List_base@VC2DLineF@@..@std@@QAE@XZ
//   (list's own ~list is the 5-byte jmp 0x00514280).
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
#include "engine/CVectorMap.h"
template void std::__push_heap(std::pair<unsigned __int64, std::list<C2DLineF> >*, int, int, std::pair<unsigned __int64, std::list<C2DLineF> >,
 CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> >);
