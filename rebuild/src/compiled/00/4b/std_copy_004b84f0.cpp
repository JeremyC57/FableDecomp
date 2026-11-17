// std::__copy<pair<CCharString, CPersistInfo>*, pair<CCharString, CPersistInfo>*, int>(first, last, result, random_access_iterator_tag, int*) -- STLport 4.6 counted random-access copy of the quest-persistence entries
// Leaf: ??$__copy@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B84F0, length 90 bytes (`__fastcall`, `ret 0Ch`).
// Identity: the element is pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> (12 bytes, the catalog's own element;
//   Ego_r.pdb CRuntimeQuestPersistence::PersistScripts); its callers CQuestManager::ClearQuestPersistScript 0x004B39B0 (NF85, the inline
//   vector::erase(first, last) of FableWin.pdb CVectorMap::EraseFromKey) and CRuntimeQuestPersistence::ClearAll 0x004B41F0 call it as
//   STLport's __copy_ptrs -> __copy. The old leaf Copy_PairCharStringPersistInfo_004b84f0 was a placeholder.
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, _Distance*)`: first in ecx, last in edx, the
//   count (last - first) counted down, three CCharString::operator= 0x0099EFB0 calls per element (the pair's implicit assignment).
// NF85: moved off the row-local SPairCharStringPersistInfo model onto stlport/fable_containers.h (FABLE_RETAIL_STLP_COPY_NOINLINE: the
//   out-of-line __fastcall body) and engine/CQuestManager.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"
template std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>* std::__copy<std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, int>(std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, const std::random_access_iterator_tag&, int*);
