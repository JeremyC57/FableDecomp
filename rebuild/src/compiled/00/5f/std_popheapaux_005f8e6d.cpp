// std::__pop_heap_aux -- remove the largest keyed entry from a heap.
// Leaf: ??$__pop_heap_aux@PAU?$pair@JW4EHeroAbility@@@std@@U12@V?$CKeyPairCompareLess@JW4EHeroAbility@@@@@std@@YIXPAU?$pair@JW4EHeroAbility@@@0@00V?$CKeyPairCompareLess@JW4EHeroAbility@@@@@Z
// Retail: 0x005F8E6D, 34 bytes, fastcall, two stack arguments.
// Flags: /Gr
#define FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22
#include "fable_compiler.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#include "engine/CVectorMap.h"
#include "engine/EHeroAbility.h"
namespace std {
template void FABLE_STLP_FASTCALL __pop_heap_aux(pair<long,EHeroAbility>*,pair<long,EHeroAbility>*,pair<long,EHeroAbility>*,CKeyPairCompareLess<long,EHeroAbility>);
}