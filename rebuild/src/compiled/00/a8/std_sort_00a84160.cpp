// std::sort<std::pair<unsigned __int64, bool>*, CKeyPairCompareLess<unsigned __int64, bool> >(first, last, comp) -- STLport 4.6: introsort
// loop to depth 2*lg(n), then the final insertion sort (inlined: threshold 16)
// Leaf: ??$sort@PAU?$pair@_K_N@std@@V?$CKeyPairCompareLess@_K_N@@@std@@YIXPAU?$pair@_K_N@0@0V?$CKeyPairCompareLess@_K_N@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A84160, 115 bytes, `ret 4` (/Gr TU: __fastcall, first in ecx, last in edx, the comparator pushed).
// Callees: __introsort_loop 0x00A83AB0, __insertion_sort 0x00A82820, __unguarded_insertion_sort_aux 0x00A7EFE0 (out-of-line __fastcall).
// NF72 (round 7, q10878): was the generic stand-in Sort_SElement16_00a84160 (catalog _global::Sort_SElement16_00a84160) on local
//   SElement16 / SCompare models. Descriptive-by-library: STLport 4.6 _algo.c sort, the instance CVectorMap<unsigned __int64, bool>::Update
//   calls (the ported CNavSwitchableLeafNode file ctor 0x00A79A30, NF68, binds this symbol through FABLE_RETAIL_STLP_SORT_FASTCALL).
//   Re-authored as the explicit instantiation of the overlay opt-in FABLE_RETAIL_STLP_SORT_FASTCALL_BODY (stlport/fable_containers.h: the
//   FABLE_RETAIL_STLP_SORT_INLINE text with sort defined __fastcall) on engine/CVectorMap.h's CKeyPairCompareLess.
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"

template void __fastcall std::sort<std::pair<unsigned __int64, bool>*, CKeyPairCompareLess<unsigned __int64, bool> >(
    std::pair<unsigned __int64, bool>*, std::pair<unsigned __int64, bool>*, CKeyPairCompareLess<unsigned __int64, bool>);
