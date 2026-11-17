// NQuestManager::CRuntimeQuestPersistence::SetQuestScriptPersistScript(const CCharString& quest_name, const CCharString& persist_script) -- record the persist script for the quest in the quest-script persist map: overwrite the entry when the quest is present, else insert {quest, info} and re-sort the map
// Leaf: ?SetQuestScriptPersistScript@CRuntimeQuestPersistence@NQuestManager@@QAEXABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B4E30, length 256 bytes (`ret 8`).
// FableWin.pdb / Ego_r.pdb (0x004BA540): ?SetQuestScriptPersistScript@CRuntimeQuestPersistence@NQuestManager@@QAEXABVCCharString@@0@Z (the
// manifest gives both setters the SetQuestMasterDataPersistScript label; the +0 map row 0x004B4E30 is SetQuestScriptPersistScript, the
// +0x10 one 0x004B4F30).
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound 0x004B9750 (the inline Find), CBasicString<char>::operator< 0x00429950 (the
// inline CCharString <), CCharString copy ctor 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0, the pair vector's push_back
// 0x004BB780, std::sort<pair*, CKeyPairCompareLess> 0x004BBB40 (`__fastcall`, the empty comparator by value).
// NF82 (q11007): moved off the row-local CCharStringData / CCharString / CPersistInfo / CVectorMap models onto the shared headers:
// fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY: the compare binds Ego_r.pdb
// ??M?$CBasicString@D@@QBE_NABV0@@Z), engine/CQuestManager.h (CRuntimeQuestPersistence's typed maps; NF82 overlay opt-in
// FABLE_RETAIL_CQUESTMANAGER_PERSISTINFO_SETTERS: the FableWin.pdb CPersistInfo constructors and the two setter declarations) and
// engine/CVectorMap.h (Find; FableWin.pdb ?Insert@?$CVectorMap@..CPersistInfo..@@QAEXABVCCharString@@ABVCPersistInfo@..@@@Z via
// FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE + _INSERT_VALUE_TYPE: the pair temporary dies before Dirty is set; then FableWin.pdb
// ?Sort@?$CVectorMap@..CPersistInfo..@@QAEXXZ via FABLE_RETAIL_CVECTORMAP_SORT), with push_back out of line
// (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE, 0x004BB780) and std::sort __fastcall (FABLE_RETAIL_STLP_SORT_FASTCALL).
// Insert then Sort, not Insert then Update: retail has no Dirty test (`mov byte ptr [esi+0Dh],1` straight into the sort's loads).
// Insert + Update matches this row too, but at 0x004B4F30 (map at +0x10) it reloads and tests Dirty, and through a map reference
// the store sinks below the loads; Insert + Sort matches both setters, so both are spelled so.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#define FABLE_RETAIL_CQUESTMANAGER_PERSISTINFO_SETTERS
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptInfoManager.h"
#include "engine/CQuestManager.h"
#include "engine/CVectorMap.h"

void NQuestManager::CRuntimeQuestPersistence::SetQuestScriptPersistScript(const CCharString& quest_name, const CCharString& persist_script)
{
    CVectorMap<CCharString, CPersistInfo>::iterator it = PersistScripts.Find(quest_name);
    CPersistInfo info(quest_name, persist_script);
    if (it != PersistScripts.end())
        it->second = info;
    else {
        PersistScripts.Insert(quest_name, info);
        PersistScripts.Sort();
    }
}
