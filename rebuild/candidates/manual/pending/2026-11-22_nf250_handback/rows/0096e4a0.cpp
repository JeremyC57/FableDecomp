// std::sort<CThing**, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> >(first, last, comp) -- STLport 4.6 sort: for a non-empty
// range __introsort_loop @ 0x0096E410 to depth 2*lg(n), then the inline __final_insertion_sort (threshold 16: __insertion_sort
// @ 0x0096D980 on the first 16, __unguarded_insertion_sort_aux @ 0x0096D780 on the rest; shorter ranges __insertion_sort only)
// Leaf: ??$sort@PAPAVCThing@@V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@std@@YIXPAPAVCThing@@0V?$CAdaptThingCompareToSTLComparitor@VCThingCompare_Nearest@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096E4A0, 112 bytes; __fastcall (first in ecx, last in edx, the 4-byte comparator
//   pushed, `ret 4`).
// Names (agentNF250, 2026-11-22, CODEXP27 L104): was the generated stand-in ?Sort_SElement4_0096e4a0@@YIXPAUSElement4@@0USCompare@@@Z
//   (local SElement4 / SCompare models, stand-in callees IntrosortLoop_0096e410 / InsertionSort_0096d980 / UnguardedInsertionSortAux_0096d780).
//   The landed caller 0x0096DF50 (CAIStateGroup_ScreamerPursueTarget::Update; Ego_r 0x00992670 / FableWin Update call sites) binds this
//   STLport spelling (descriptive-by-library: the PDB name sets hold no STL instance names). Now the STLport body, emitted by an explicit
//   instantiation under stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_FASTCALL + _BODY (the 0x00A84160 pattern); the comparator from
//   engine/CAdaptThingCompareToSTLComparitor.h / engine/CThingCompare_Nearest.h (the caller's headers).
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAdaptThingCompareToSTLComparitor.h"
class CThing;

template void __fastcall std::sort<CThing**, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> >(
    CThing**, CThing**, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>);
