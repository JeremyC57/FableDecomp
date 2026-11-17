// std::sort<pair<CCharString, CSoundBankLH*>*, CKeyPairCompareLess<CCharString, CSoundBankLH*> >(first, last, comp) -- STLport 4.6 introsort of the sound manager's bank map pairs
// Leaf: ??$sort@PAU?$pair@VCCharString@@PAVCSoundBankLH@@@std@@V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@std@@YIXPAU?$pair@VCCharString@@PAVCSoundBankLH@@@0@0V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41E20, length 115 bytes (`__fastcall`, `ret 4`).
// Identity: STLport 4.6 _algo.c sort over pair<CCharString, CSoundBankLH*> (8-byte elements; RTTI .?AVCSoundBankLH@@): the landed caller
//   CSoundManagerLH::NewAtmos 0x00A3F140 (+0xFC, probed) binds exactly this spelling, and the out-of-line callees are the same instance's
//   __introsort_loop / __insertion_sort / __unguarded_insertion_sort_aux (descriptive-by-library). The old row's own name
//   _global::Sort_SElement8_00a41e20 (SElement8 / SCompare models) was a stand-in.
// Shape: the empty-range test, __introsort_loop (out of line), then the inline __final_insertion_sort (threshold 16) over the two
//   out-of-line insertion-sort helpers.
// NF87: moved onto stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_FASTCALL + _SORT_FASTCALL_BODY, NF72's 0x00A84160 recipe) and
//   engine/CVectorMap.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CSoundBankLH;   // RTTI .?AVCSoundBankLH@@ (no header: declared only)
#include "engine/CVectorMap.h"
template void __fastcall std::sort<std::pair<CCharString, CSoundBankLH*>*, CKeyPairCompareLess<CCharString, CSoundBankLH*> >(
    std::pair<CCharString, CSoundBankLH*>*, std::pair<CCharString, CSoundBankLH*>*, CKeyPairCompareLess<CCharString, CSoundBankLH*>);
