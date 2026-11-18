// std::__adjust_heap(pair<CCharString, CScriptInfo>* first, int holeIndex, int len, pair<CCharString, CScriptInfo> val, CKeyPairCompareLess<CCharString, CScriptInfo> comp) -- STLport 4.6 _heap.c sift-down of the script-info registry sort (CScriptInfoManager's CVectorMap<CCharString, CScriptInfo>)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIXPAU?$pair@VCCharString@@VCScriptInfo@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB68C0, 310 bytes, __fastcall (the /Gr algorithm TU: first in ecx, holeIndex in edx).
// Identity: the landed __make_heap 0x00CB6BD0 of the same sort binds 0x00CB68C0 under this exact STLport spelling (reloc_bindings);
//   the 24-byte element is pair<CCharString, CScriptInfo> (Ego_r.pdb CScriptInfo 0x14: AllocFunc, MasterScriptDataAlloc, Name, ID,
//   MasterScript; engine/CScriptInfo.h). The catalog's "_Adjust_heap<..>" is a Dinkumware (FableWin) spelling; retail is STLport.
// Callees: CBasicString<char>::operator< 0x00429950 (the inline CCharString compare), the CCharString copy / assignment / destructor,
//   and the out-of-line __push_heap of the same instantiation.
// NF80 (q10923): moved off the stand-in namespace N00cb68c0 (local CBasicString / CCharString / CWideString / Element / comparator /
//   counted-pointer models) onto stlport/fable_containers.h (FABLE_RETAIL_STLP_DECAL_HEAP, /Gr: the 0x00BEEA00 recipe),
//   engine/CScriptInfo.h, engine/CVectorMap.h (CKeyPairCompareLess) and fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<CCharString, CScriptInfo>*, int, int, std::pair<CCharString, CScriptInfo>, CKeyPairCompareLess<CCharString, CScriptInfo>);
