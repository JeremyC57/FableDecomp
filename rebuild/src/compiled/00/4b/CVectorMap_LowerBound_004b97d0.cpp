// CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo, CKeyPairCompareLess<..> >::LowerBound(const CCharString& key) const -- binary search of the sorted 12-byte quest persistence entries for the first entry whose key is not less than key
// Leaf: ?LowerBound@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@@QBEPBU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B97D0, 128 bytes, `__fastcall`, `ret 4`, the entry pointer (STLport iterator) in eax.
// Instance: a second CQuestManager 12-byte-entry map (callers CQuestManager::AttachExistingScriptedEntitiesToScript 0x004B26B0, CQuestManager::ActivateQuests 0x004B3CE0); byte-identical to 0x004B9750 apart from the call displacement. The manifest's value types are propagated across byte-identical bodies (the
//   entry stride here is 12 bytes): the payload after the CCharString key is Ego_r.pdb CPersistInfo (QuestName, PersistScript).
// Callees: CBasicString<char>::operator< @ 0x00429950 (the string data compare behind the inline CCharString::operator<).
// Body: Dinkumware-style lower/upper bound -- count = (last - first) / sizeof(entry); halve, compare the middle entry,
//   step past it (`count -= half + 1`, emitted `or -1 ; sub ; add`) or keep the lower half.
// Shape: the key's string-data pointer is read ONCE after the empty-map test (the call could alias a re-read), so the
//   key is copied into a local inside `if (count > 0) { ...; do { } while (count > 0); }` -- a copy before the test
//   is scheduled above the `jle`. The inline CCharString::operator< tests `other.PData == PData`, then a null left,
//   then a null right, then calls the data compare (the same order as std::less<CCharString> 0x0042AAB3).
// Order: the count is computed before `first` is copied from First (the other order loads First straight into ebp
//   and drops retail's `mov ebp,esi`).
// Layout: STLport vector {first, last, end_of_storage}; entry pair<CCharString, value>, key at +0 (one data pointer).
// NF82 (q11003): moved off the row-local CBasicString / CCharString / CQuestMapValue / pair / CKeyPairCompareLess / CVectorMap models
//   onto engine/CVectorMap.h (the out-of-line const LowerBound body, explicit instantiation), engine/CQuestManager.h (Ego_r.pdb
//   NQuestManager::CRuntimeQuestPersistence::CPersistInfo, the value of PersistScripts / MasterDataPersistScripts) and fable_string.h
//   (CBASICSTRING_LESS_BODY: the visible compare lets VC7.1 read the key once, as retail); the quest-manager TU preamble of NF80's
//   0x004B27F0. The catalog's CVectorMap<CCharString, CQuestMapValue, ..> was a stand-in value type; the own symbol is now the const
//   Ego_r.pdb instance NF80's 0x004B27F0 and the landed 0x004B3CE0 bind.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CSCRIPTINFO_ACCESSORS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_COUNT_FIRST
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUESTS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LOADTHINGSWITHSECTIONNAMES
#define FABLE_RETAIL_CWORLDMAP_UPDATENAVMAPS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CWorld.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptBase.h"
#include "engine/CScriptDataBase.h"
#include "engine/CMessageEventManager.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CQuestManager.h"
#include "engine/CWorldMap.h"
template CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::const_iterator CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::LowerBound(const CCharString&) const;
