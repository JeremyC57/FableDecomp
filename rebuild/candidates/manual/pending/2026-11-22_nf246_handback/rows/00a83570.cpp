// std::__pop_heap<pair*, ..> (STLport 4.6 _heap.c / _algo.c, element std::pair<unsigned __int64, std::list<C2DLineF> > (the navigation layer switchable line lists)) -- explicit instantiation of the shared STLport template
// Leaf: ??$__pop_heap@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@U12@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@H@std@@YIXPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@00U10@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83570, 141 bytes.
// NF95 (part 2, q11353/q11354 binder port): was a global-scope stand-in (?__pop_heap@@YI..) over a row-local model; now the std:: template
//   instantiated on the shared headers so the symbol is the retail std:: spelling (Ego_r.pdb template names; the 0x004E1D20 /
//   0x004E1D68 / 0x005E2EB0 / 0x005E27F0 recipes) and the callees bind under their std:: names (__adjust_heap self rows 0x005E1CB0 /
//   0x00A82BE0). Graded without /Gr: every heap function is declared FABLE_STLP_FASTCALL in the header.
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
// NF246 (2026-11-22, the 0x005127A0 family, nf243 NOTES section 2): FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE (a noinline `~list() {}`, so the
//   leaf called ~list and bound it at 0x005127A0) -> FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE (NF241's swap): ~list inline, the
//   leaf calls ~_List_base<C2DLineF> 0x005127A0 out of line, as retail (the 5-byte ~list jmp is 0x00514280).
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
#include "engine/CVectorMap.h"
template void std::__pop_heap(std::pair<unsigned __int64, std::list<C2DLineF> >*, std::pair<unsigned __int64, std::list<C2DLineF> >*, std::pair<unsigned __int64, std::list<C2DLineF> >*,
 std::pair<unsigned __int64, std::list<C2DLineF> >, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> >, int*);
