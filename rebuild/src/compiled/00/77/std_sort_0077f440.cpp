// std::sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector> >(first, last, comp) -- STLport 4.6 introsort of a CVectorMap<CCharString, C3DVector>'s pairs
// Leaf: ??$sort@PAU?$pair@VCCharString@@VC3DVector@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@0V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F440, length 115 bytes (`__fastcall`, `ret 4`).
// Identity: STLport 4.6 _algo.c sort over pair<CCharString, C3DVector> (16-byte elements): its out-of-line callees are the same instance's
//   __introsort_loop 0x0077F370 (q11131), __insertion_sort 0x0077ECB0 (q11130) and __unguarded_insertion_sort_aux 0x0077EA10, all NF85
//   ports on this element (descriptive-by-library: the spelling is what VC7.1 emits for STLport 4.6 over the element the callees prove).
//   The old _global::Sort_SElement16_0077f440 name and its SElement16 / SCompare models were stand-ins.
// Shape: the empty-range test, __introsort_loop (out of line, the (T*)0, depth __lg(n) * 2 and the comparator pushed), then the inline
//   __final_insertion_sort (threshold 16) over the two out-of-line insertion-sort helpers.
// NF87: moved onto stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_FASTCALL + _SORT_FASTCALL_BODY, NF72's 0x00A84160 recipe) and
//   engine/CVectorMap.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort<std::pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector> >(
    std::pair<CCharString, C3DVector>*, std::pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>);
