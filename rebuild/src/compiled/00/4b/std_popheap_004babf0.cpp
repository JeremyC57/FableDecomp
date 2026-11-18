// std::__pop_heap<std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, CKeyPairCompareLess<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>> (STLport 4.6, element std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>) -- SGI pop_heap core for the quest persistence (name, persist info) heap: *result = *first, then sift `value` down from the root
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@H@std@@YIXPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@00U10@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BABF0, length 158 bytes (+ int3 padding), `ret 18h` (result, the 0xc-byte
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>, comparator CKeyPairCompareLess<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*,std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*,std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*,std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>,CKeyPairCompareLess<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>,int*);
