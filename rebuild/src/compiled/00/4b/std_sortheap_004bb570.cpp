// std::sort_heap<pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, CKeyPairCompareLess<..> >(first, last, comp) -- STLport 4.6 sort_heap over the quest-persistence table: pop the root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@std@@YIXPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BB570, length 143 bytes (+ int3 padding), `ret 4`.
// Identity: element pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> (12 bytes: stride 0x2AAAAAAB / sar 1; Ego_r.pdb
//   CRuntimeQuestPersistence::PersistScripts); its callee 0x004BABF0 is the STLport __pop_heap of the same pair (NF85), and its retail caller
//   std::__partial_sort 0x004BB810 already binds this spelling. The catalog label `_Sort_heap<pair<unsigned long,CSmallVector<..>>*,...>`
//   was propagated over the byte-identical skeleton, and the old row's own name was a stand-in.
// Callees: __pop_heap @ 0x004BABF0 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x3).
// Shape: /Gr STLport TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and __pop_heap_aux inline.
// NF87: moved off the row-local CCharString / CRuntimeQuestPersistence / SQuestPersistPair models onto stlport/fable_containers.h
//   (FABLE_RETAIL_STLP_SORT_HEAP_BODY, NF85) and engine/CQuestManager.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort_heap(std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, CKeyPairCompareLess<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>);
