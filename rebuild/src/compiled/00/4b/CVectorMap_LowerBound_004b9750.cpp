// CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::LowerBound(const CCharString& key) -- binary search of the sorted 12-byte (key, CPersistInfo) entries for the first entry whose key does not sort below key
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@@QAEPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B9750, 121 bytes (+ padding), thiscall `ret 4`, the entry pointer in eax.
// Identity (NF82 q11087, checked by NF85): FableWin.pdb ?LowerBound@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@..
//   (the CRuntimeQuestPersistence::PersistScripts map, Ego_r.pdb); its landed callers CRuntimeQuestPersistence::SetQuestScriptPersistScript
//   0x004B4E30 / SetQuestMasterDataPersistScript 0x004B4F30 (NF82) bind this spelling, and retail 0x004B9750 is byte-identical to the const
//   LowerBound 0x004B97D0 of the same map (NF82) apart from the operator< call displacement. The old row's CQuestMapValue was a stand-in.
// Callees: CBasicString<char>::operator< @ 0x00429950 (the string data compare behind the inline CCharString::operator<).
// NF85 (q11087): moved off the row-local CBasicString / CCharString / pair / comparator / CVectorMap models onto engine/CVectorMap.h (the
//   out-of-line non-const LowerBound, explicit instantiation, with the NF85 opt-in FABLE_RETAIL_CVECTORMAP_LOWERBOUND_COUNT_FIRST: the count
//   taken before `first`, the non-const twin of NF82's const opt-in), engine/CQuestManager.h (CPersistInfo) and fable_string.h, with the
//   NF82 0x004B97D0 opt-in set.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_CVECTORMAP_LOWERBOUND_COUNT_FIRST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"
template CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::iterator CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::LowerBound(const CCharString&);
