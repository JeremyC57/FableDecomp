// std::sort(std::pair<CCharString, CScriptInfo>* first, std::pair<CCharString, CScriptInfo>* last, CKeyPairCompareLess<CCharString, CScriptInfo> comp) -- STLport sort of CScriptInfoManager's script table: when the range is not empty, introsort it with depth limit 2 * floor(log2(n)), then the final insertion sort
// Leaf: ??$sort@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIXPAU?$pair@VCCharString@@VCScriptInfo@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7780, 83 bytes, __fastcall (first in ecx, last in edx, the 4-byte comparator pushed), `ret 4`.
// Identity: STLport 4.6 _algo.c sort(first, last, comp) for the 24-byte pair<CCharString, CScriptInfo> (n by the magic divide, the inline
//   __lg loop, depth `lea eax,[ecx+ecx]`), as the landed family-L050 sorts 0x00CBD370 / 0x00CBDF70. Callees: __introsort_loop
//   0x00CB7310 (the landed ??$__introsort_loop@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@U12@HV?$CKeyPairCompareLess@...) and
//   __final_insertion_sort 0x00CB70E0 (landed ??$__final_insertion_sort@...@std@@YIX...), both __fastcall and out of line. Callers:
//   CScriptInfoManager::Init 0x00CB5D80 (the inline sort-if-dirty of its Scripts map) and the CVectorMap<CCharString, CScriptInfo,
//   CKeyPairCompareLess<CCharString, CScriptInfo> > members 0x00CB77E0 / 0x00CB7800.
// Naming (NF180, 2026-11-20, q4221): was the free stand-in Sort_SElement24_00cb7780 over 24-byte SElement24 blobs and an SCompare
//   functor; now the STLport instance on the shared headers (stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_FASTCALL_BODY with the
//   NF180 nested FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE, engine/CScriptInfo.h, engine/CVectorMap.h), so both callee
//   relocations bind the self rows' spellings. Descriptive-by-library (STLport instances are in neither PDB).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#define FABLE_RETAIL_STLP_SORT_FINAL_INSERTION_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CScriptInfo.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort(std::pair<CCharString, CScriptInfo>*, std::pair<CCharString, CScriptInfo>*, CKeyPairCompareLess<CCharString, CScriptInfo>);
