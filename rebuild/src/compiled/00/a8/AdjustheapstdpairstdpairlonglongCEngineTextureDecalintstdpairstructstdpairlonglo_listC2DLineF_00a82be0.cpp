// std::__adjust_heap(pair<unsigned __int64, list<C2DLineF> >* first, int holeIndex, int len, pair<unsigned __int64, list<C2DLineF> > val, CKeyPairCompareLess<unsigned __int64, list<C2DLineF> > comp) -- STLport heap sift-down of the navigation layer's switchable line lists (unsigned 64-bit key) to a leaf, then __push_heap of val back up
// Leaf: ??$__adjust_heap@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@HU12@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@std@@YIXPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@HHU10@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A82BE0, 258 bytes, __fastcall (/Gr algorithm TU: first in ecx, holeIndex in edx; len, the
//   16-byte value and the empty comparator on the stack), `ret 18h`.
// Identity: STLport 4.6 _heap.c __adjust_heap over Ego_r.pdb CNavigationLayer::SwitchableLineLists' value_type (the unsigned 64-bit key:
//   high dword compared first `ja`/`jb`, then the low `jae`; the one-word STLport list<C2DLineF> +8). Callers: the landed __pop_heap
//   0x00A83570 and __make_heap 0x00A83680; callee the landed __push_heap 0x00A820E0. The catalog label "_Adjust_heap<pair<pair<long,
//   long>,CEngineTextureDecal>..." is propagated from a byte-identical body (wrong element).
// Source: the shared STLport body (fable_containers.h FABLE_RETAIL_STLP_DECAL_HEAP), explicit instantiation, with the STLport list as
//   this TU calls it: list::operator= out of line (0x00517720, FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL), the copy constructor's
//   range insert _M_insert_dispatch out of line (0x005152C0, FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE), ~list out of line
//   (0x005127A0, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE). This closes the parked val-copy scheduling residue (local list model).
// Callees: list<C2DLineF>::operator= 0x00517720, _M_insert_dispatch 0x005152C0, malloc 0x00BFEA0E (val copy's sentinel),
//   __push_heap 0x00A820E0, list<C2DLineF>::~list 0x005127A0.
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<unsigned __int64, std::list<C2DLineF> >*, int, int, std::pair<unsigned __int64, std::list<C2DLineF> >, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> >);