// CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::UpperBound(const CCharString& key) -- binary search of the sorted 12-byte (key, CPersistInfo) entries for the first entry whose key sorts after the key
// Leaf: ?UpperBound@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@@QAEPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9850, 121 bytes, thiscall `ret 4`, the entry pointer in eax.
// Identity: the CRuntimeQuestPersistence::PersistScripts map (Ego_r.pdb CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess<..> >;
//   FableWin.pdb ?EraseFromKey@?$CVectorMap@VCCharString@@VCPersistInfo@..: LowerBound then UpperBound): its caller
//   CQuestManager::ClearQuestPersistScript 0x004B39B0 calls it right after LowerBound 0x004B9750 on the same map, and it is that
//   LowerBound's byte twin but for the compare direction. The old row's CQuestMapValue (and its pair<.., CQuestMapValue>) was a stand-in.
// Callees: CBasicString<char>::operator< @ 0x00429950 (the string data compare behind the inline CCharString::operator<).
// NF85 (claimed, from q11087): moved off the row-local CBasicString / CCharString / pair / CVectorMap models onto engine/CVectorMap.h
//   (FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND's body with the NF85 opt-in FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST: out of line,
//   the count taken before `first`), engine/CQuestManager.h (CPersistInfo) and fable_string.h; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_CVECTORMAP_PODIUM_UPPERBOUND
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND_COUNT_FIRST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"
template CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::iterator CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::UpperBound(const CCharString&);
