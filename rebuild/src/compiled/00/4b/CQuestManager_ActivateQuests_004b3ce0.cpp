// CQuestManager::ActivateQuests(const CArray<CActivateQuestInfo>& quests) -- activate a batch of quests. Pass 1 (resources): for every
// quest that is neither active nor already resource-loaded, queue its name for LoadResourcesForQuests and remember it in
// QuestsWithResourcesLoadedThatAreNotActivated (when the record asks for resources), and load the quest's code section when one is
// registered under the quest name; then load the queued resources. Pass 2 (activation): for every quest still inactive, look up its
// persisted script text, then create its CActiveQuest -- with a script when the record has a script info and quest scripts are
// enabled (master data: allocated + Init + LoadMasterScriptDataFromGameState for a master script, else GetMasterScriptDataForScript;
// script from the info's allocator; Activate; restore the persisted text; MESSAGE_EVENT_QUEST_ACCEPTED), else a bare record --
// append it to ActiveQuests and drop the name from the resource-loaded set.
// Leaf: ?ActivateQuests@CQuestManager@@AAEXABV?$CArray@VCActivateQuestInfo@CQuestManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B3CE0, 1285 bytes (int3 padded to 0x004B41F0), __fastcall, `ret 4`. No flags (speed TU, no EH
//   frame, no frame pointer).
// Identity: Ego_r.pdb / FableWin.pdb ?ActivateQuests@CQuestManager@@AAEXABV?$CArray@VCActivateQuestInfo@CQuestManager@@@@@Z (private);
//   its one caller is ActivateMultipleQuests 0x004B4260 (call at 0x004B4386).
// Callees: CQuestManager::IsQuestActive 0x004AF610; set<CCharString> _M_find 0x004B9370, insert_unique 0x004AC380, erase(key)
//   0x004BB270 (called in the scripted arm, expanded in the bare arm: _M_upper_bound 0x004B9D50, _M_lower_bound 0x004B9D00,
//   erase(iterator, iterator) 0x004B9C10; the distance loop of the expanded copy is dead and removed);
//   vector<CCharString>::_M_insert_overflow 0x00433530 (push_back growth); CCharString copy 0x0099EC30 / dtor 0x0099EAE0;
//   CTBaseSingleton<CCodeSectionManager>::Get expanded (instance 0x013CAA68, operator new 0x00BFEA1A, CCodeSectionManager ctor
//   0x009F0320, CCountedPointer<CCodeSectionManager>::Reset 0x00413590); CCodeSectionManager::IsSectionRegisteredLongName 0x009F0570,
//   GetHandleFromLongName 0x009F0500, LoadSection 0x009F02C0 (LoadSectionLongName expanded); LoadResourcesForQuests 0x004B2510;
//   CVectorMap<CCharString, CPersistInfo>::LowerBound 0x004B97D0 and CBasicString<char>::operator< 0x00429950 (the expanded
//   GetQuestScriptPersistScript / Find / CCharString::operator<); LoadMasterScriptDataFromGameState 0x004B3760;
//   GetMasterScriptDataForScript 0x004AFA10; CActiveQuest 6-argument ctor 0x004B0310 (the 3-argument one is expanded);
//   CCountedPointer<CActiveQuest>::DeleteData 0x004BAEF0 (address stored in the count block); malloc 0x00BFEA0E (list node);
//   CScriptBase::Activate 0x00CB7900 / LoadGameState 0x00CB8690; GetIndexFromRegisteredQuest 0x004AF740;
//   CMessageEventManager::AddMessage 0x00687540; operator delete 0x00BFE9BC; free 0x00BFEA14 (the inline ~vector).
//   Data: g_QuestScriptsEnabled 0x01375454 (bool; DESCRIPTIVE name -- found in no PDB, one reader, 0x004B3EED).
// Shape (each point is what the bytes need):
//   * Both loops are `while (i < quests.size()) { ...; ++i; }` (the second with `if (!IsQuestActive(..)) { .. }`, no continue): c2's
//     static frequencies for that form are what make VC7.1 call set::erase(key) in the scripted arm (`a && b` = the colder arm) and
//     expand it in the bare arm. `for` loops expand both (1304 bytes); no /d2inl flag is needed with `while`.
//   * CArray::operator[] goes through begin() (FABLE_RETAIL_CARRAY): `lea esi,[edx+ecx]` (offset + start).
//   * The script info is read through its inline PDB accessors IsMasterScript / GetMasterScriptDataAllocFunc / GetAllocFunc (overlay
//     opt-in FABLE_RETAIL_CSCRIPTINFO_ACCESSORS; both allocators are __fastcall, the script allocator takes the interface by
//     reference): GetAllocFunc()'s returned pointer is a value of its own (`mov eax,[ecx] ; mov ecx,<data> ; call eax`, 0x004B3F43),
//     and with the accessors spelled the constant-zero register takes ebx from the lookup to the end of the bare arm
//     (0x004B3EA9 `xor ebx,ebx`; direct member reads leave ebx to the bare arm's quest id: 1289 bytes).
//   * The quest id comes from the inline AllocateQuestID() (both PDBs; `mov eax,[this+84h] ; .. ; inc eax ; mov [this+84h],eax`).
//   * `master_data = script_data` is assigned before LoadMasterScriptDataFromGameState (0x004B3F27 `mov ebx,esi` ahead of the call).
//   * The message manager is reached through the member (World +0x7C, manager +0x60) AFTER GetIndexFromRegisteredQuest returns
//     (0x004B402B); the inline CWorld::GetMessageEventManager() accessor is evaluated before the call instead.
//   * Callee bodies VC7.1's TU saw (all REAL bodies, none a stand-in):
//       - CQuestManager::LoadResourcesForQuests 0x004B2510 (landed row, same retail source file, defined earlier) and STLport's
//         vector::_M_insert_overflow (header body): with both visible c2 knows the local array's address is never kept, so
//         _M_finish stays in edi across the CCharString copy constructor (0x004B3D58..0x004B3D71) and the frame is 0x34;
//       - CBasicString<char>::operator< 0x00429950 (landed row): pure reads, so the map's end() is read once (0x004B3EA4) and
//         compared again after the call from its frame home (0x004B3ECB).
//     _M_find / _M_lower_bound / _M_upper_bound / erase(iterator, iterator) are declarations (retail's COMDATs follow this function).
// Layout: engine/CQuestManager.h (FABLE_RETAIL_CQUESTMANAGER + _LOADGAMESTATE + _ACTIVEQUEST + _SAVEGAMESTATE + the wf1 overlay opt-in
//   _ACTIVATEMULTIPLEQUESTS + this row's _ACTIVATEQUESTS: DayActive / NightActive typed for LoadResourcesForQuests, AllocateQuestID);
//   engine/CScriptInfo.h (overlay opt-in FABLE_RETAIL_CSCRIPTINFO_ACCESSORS); stlport/fable_map.h (overlay opt-in
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE). CCodeSectionManager and CTCDayOrNightOnlySupport have no shared header (declarations
//   only here).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
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

// CBasicString<char>::operator< 0x00429950 (landed): its real body in the TU, kept out of line, as retail's TU sees it.
#pragma auto_inline(off)
template<> __declspec(noinline) bool CBasicString<char>::operator<(const CBasicString<char>& other) const
{
    const char* b=other.PData;
    const char* a=PData;
    for(;;) {
        if(*a==0 && *b==0) return false;
        if(*a<*b) return true;
        if(*a>*b) return false;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCodeSectionManager : public CTBaseSingleton<CCodeSectionManager> {   // 0x2c bytes; the instance pointer is 0x013CAA68
public:
    CCodeSectionManager();                                    // 0x009F0320
    bool IsSectionRegisteredLongName(CCharString long_name);  // 0x009F0570 (both PDBs: ?IsSectionRegisteredLongName@CCodeSectionManager@@QAE_NVCCharString@@@Z)
    unsigned long GetHandleFromLongName(CCharString long_name);   // 0x009F0500
    void LoadSection(unsigned long handle);                   // 0x009F02C0
    // ?LoadSectionLongName@CCodeSectionManager@@QAEXVCCharString@@@Z: landed out-of-line COMDAT 0x004AF000
    // (rebuild/src/compiled/00/4a/CCodeSectionManager_LoadSectionLongName_004af000.cpp); expanded in ActivateQuests 0x004B3DE3..0x004B3E0D.
    void LoadSectionLongName(CCharString long_name) { LoadSection(GetHandleFromLongName(long_name)); }
private:
    unsigned char _pad_0x04[0x2c - 0x04];
};
extern bool g_QuestScriptsEnabled;                            // 0x01375454 (descriptive)

class CTCDayOrNightOnlySupport {                              // the day/night-only suffix statics (no shared header)
public:
    static const CCharString DAY_ONLY_SUFFIX;                 // 0x013BAC18 "DayOnly"
    static const CCharString NIGHT_ONLY_SUFFIX;               // 0x013BAC1C "NightOnly"
};

// 0x004B2510 CQuestManager::LoadResourcesForQuests (landed: rebuild/src/compiled/00/4b/CQuestManager_LoadResourcesForQuests_004b2510.cpp),
// defined ahead of ActivateQuests as in the retail source file: its real body on the shared headers, kept out of line.
#pragma auto_inline(off)
void CQuestManager::LoadResourcesForQuests(const CArray<CCharString>& quests)
{
    std::set<CCharString> sections;
    for (unsigned int i = 0; i < quests.size(); ++i) {
        const CCharString& section = quests[i];
        sections.insert(section);
        if (DayActive) {
            CCharString day_name = section + "%" + CTCDayOrNightOnlySupport::DAY_ONLY_SUFFIX;
            sections.insert(day_name);
        }
        if (NightActive) {
            CCharString night_name = section + "%" + CTCDayOrNightOnlySupport::NIGHT_ONLY_SUFFIX;
            sections.insert(night_name);
        }
    }
    World->GetWorldMap().LoadThingsWithSectionNames(sections, CWorldMap::WAIT_FOR_LOAD);
    World->GetWorldMap().UpdateNavMaps(sections, CWorldMap::WAIT_FOR_LOAD);
}
#pragma auto_inline(on)

void CQuestManager::ActivateQuests(const CArray<CActivateQuestInfo>& quests)
{
    CArray<CCharString> quests_to_load_resources_for;
    unsigned int i = 0;
    while (i < quests.size()) {
        const CActivateQuestInfo& info = quests[i];
        if (!IsQuestActive(info.QuestName) &&
            QuestsWithResourcesLoadedThatAreNotActivated.find(info.QuestName) == QuestsWithResourcesLoadedThatAreNotActivated.end()) {
            if (info.LoadResources) {
                quests_to_load_resources_for.push_back(info.QuestName);
                QuestsWithResourcesLoadedThatAreNotActivated.insert(info.QuestName);
            }
            CCodeSectionManager& sections = CCodeSectionManager::Get();
            if (sections.IsSectionRegisteredLongName(info.QuestName)) {
                sections.LoadSectionLongName(info.QuestName);
            }
        }
        ++i;
    }
    if (!quests_to_load_resources_for.empty())
        LoadResourcesForQuests(quests_to_load_resources_for);
    unsigned int j = 0;
    while (j < quests.size()) {
        const CActivateQuestInfo& info = quests[j];
        if (!IsQuestActive(info.QuestName)) {
            const CCharString* persist_script = RuntimeQuestPersistence.GetQuestScriptPersistScript(info.QuestName);
            if (info.PScriptInfo && g_QuestScriptsEnabled) {
                long id = AllocateQuestID();
                CScriptDataBase* script_data = 0;
                CScriptDataBase* master_data;
                if (info.PScriptInfo->IsMasterScript()) {
                    script_data = info.PScriptInfo->GetMasterScriptDataAllocFunc()();
                    script_data->Init();
                    master_data = script_data;
                    LoadMasterScriptDataFromGameState(*script_data, info.QuestName);
                }
                else
                    master_data = GetMasterScriptDataForScript(*info.PScriptInfo);
                CScriptBase* script = info.PScriptInfo->GetAllocFunc()(master_data, (CGameScriptInterfaceBase&)World->GetGameScriptInterface());
                CCountedPointer<CActiveQuest> quest(new CActiveQuest(id, info.QuestName, *info.PScriptInfo, script, script_data, info.LoadResources));
                ActiveQuests.push_back(quest);
                QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
                script->Activate();
                if (persist_script)
                    quest->LoadFromGameState(*persist_script);
                World->PMessageEventManager_Data->AddMessage(MESSAGE_EVENT_QUEST_ACCEPTED, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                                                           GetIndexFromRegisteredQuest(info.QuestName));
            }
            else {
                long id = AllocateQuestID();
                CCountedPointer<CActiveQuest> quest(new CActiveQuest(id, info.QuestName, info.LoadResources));
                ActiveQuests.push_back(quest);
                QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
                if (persist_script)
                    quest->LoadFromGameState(*persist_script);
            }
        }
        ++j;
    }
}
