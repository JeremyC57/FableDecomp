// std::__make_heap<pair*, ..> (STLport 4.6 _heap.c / _algo.c, element std::pair<unsigned __int64, std::list<C2DLineF> > (the navigation layer switchable line lists)) -- explicit instantiation of the shared STLport template
// Leaf: ??$__make_heap@PAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@std@@V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@U12@H@std@@YIXPAU?$pair@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@0@0V?$CKeyPairCompareLess@_KV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@@@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83680, 156 bytes.
// NF95 (part 2, q11353/q11354 binder port): was a global-scope stand-in (?__make_heap@@YI..) over a row-local model; now the std:: template
//   instantiated on the shared headers so the symbol is the retail std:: spelling (Ego_r.pdb template names; the 0x004E1D20 /
//   0x004E1D68 / 0x005E2EB0 / 0x005E27F0 recipes) and the callees bind under their std:: names (__adjust_heap self rows 0x005E1CB0 /
//   0x00A82BE0). Graded without /Gr: every heap function is declared FABLE_STLP_FASTCALL in the header.
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
#include "engine/CVectorMap.h"
template void std::__make_heap(std::pair<unsigned __int64, std::list<C2DLineF> >*, std::pair<unsigned __int64, std::list<C2DLineF> >*, CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> >, std::pair<unsigned __int64, std::list<C2DLineF> >*, int*);
