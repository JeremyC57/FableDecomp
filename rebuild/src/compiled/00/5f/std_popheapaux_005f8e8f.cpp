// std::__pop_heap_aux -- remove the largest keyed entry from a heap.
// Leaf: ??$__pop_heap_aux@PAU?$pair@JJ@std@@U12@V?$CKeyPairCompareLess@JJ@@@std@@YIXPAU?$pair@JJ@0@00V?$CKeyPairCompareLess@JJ@@@Z
// Retail: 0x005F8E8F, 34 bytes, fastcall, two stack arguments.
// Flags: /Gr
#define FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22
#include "fable_compiler.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#include "engine/CVectorMap.h"
namespace std {
template void FABLE_STLP_FASTCALL __pop_heap_aux(pair<long,long>*,pair<long,long>*,pair<long,long>*,CKeyPairCompareLess<long,long>);
}