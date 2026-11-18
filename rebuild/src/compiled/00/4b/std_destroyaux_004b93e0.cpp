// std::__destroy_aux<pair<CCharString, CPersistInfo>*>(first, last, __false_type) -- STLport 4.6 range destructor of the quest-persistence entries
// Leaf: ??$__destroy_aux@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@@std@@YIXPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B93E0, 51 bytes (`__fastcall`, `ret 4`: first in ecx, last in edx, the tag pushed).
// Identity: the element is pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> (12 bytes: three ~CCharString
//   0x0099EAE0 calls per element, +8 / +4 / +0); its caller CQuestManager::ClearQuestPersistScript 0x004B39B0 (NF85) reaches it through
//   the inline vector::erase(first, last) of FableWin.pdb CVectorMap::EraseFromKey on CRuntimeQuestPersistence::PersistScripts. The
//   catalog's vector::_Destroy and the old row's 0x0047BAF0 Dinkumware _Destroy model were stand-ins (retail is STLport 4.6).
// NF85: moved off the row-local pair / CCharString model onto stlport/fable_containers.h (FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL:
//   the out-of-line __fastcall body) and engine/CQuestManager.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"
template void std::__destroy_aux<std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*>(std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, const std::__false_type&);
