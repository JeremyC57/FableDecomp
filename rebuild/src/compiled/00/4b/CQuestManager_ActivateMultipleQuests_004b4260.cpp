// CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool game_script, bool load_resources) -- show the
// "QuestManager: Activate Quest" progress text for every name, queue each REGISTERED quest with its script info (or none) and the two
// flags, activate the queued quests together; true when at least one name was registered.
// Leaf: ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B4260, 359 bytes, __fastcall, `ret 0Ch`, bool in al. No flags (speed TU, no EH state, no
//   frame pointer).
// Identity: Ego_r.pdb / FableWin.pdb ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z (public). The
//   second argument lands in CActivateQuestInfo::GameScript (+8), the third in LoadResources (+9) (CGameScriptInterface::
//   ActivateMultipleQuests 0x00892EA0 passes (true, true), ...WithoutLoadingResources 0x00892EE0 passes (true, false)); the parameter
//   names are descriptive.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("QuestManager: Activate Quest" 0x01239440), NProgressDisplay::DisplayProgress
//   0x009E9F40 (__fastcall), ~CCharString 0x0099EAE0, CQuestManager::IsQuestRegistered 0x004B00C0, CScriptInfoManager::GetPScriptInfo
//   0x00CB5AD0 (ScriptInfoManager +0x78), CCharString copy ctor 0x0099EC30 (the inline CActivateQuestInfo constructor),
//   std::vector<CQuestManager::CActivateQuestInfo>::push_back 0x004BB720 (out-of-line COMDAT), CQuestManager::ActivateQuests
//   0x004B3CE0, free 0x00BFEA14 (the inline ~vector: ~CCharString over [start, finish) at stride 0xC, then the storage).
// Shape: (1) the progress text is a NAMED block-scoped CCharString (constructed before DisplayProgress's arguments are pushed; VC7.1
//   homes it in the dead quest_names argument slot); (2) `activated = true` is written in BOTH arms of the script-info test (the arms
//   differ only in the PScriptInfo argument, two distinct 12-byte temporaries; VC7.1 merges the stores behind the shared ~CCharString);
//   (3) the private ActivateQuests is defined EARLIER in the retail source file (0x004B3CE0 < 0x004B4260): with its body visible VC7.1
//   knows the local array's address is not kept, which is what puts `activated` in bl, `this` in edi, quest_names in ebp, leaves the
//   index in the frame and keeps the array start in ebp across the destructor loop. A declaration alone gives 349 v 359 bytes;
//   (4) retail calls vector<CActivateQuestInfo>::push_back out of line at both sites (0x004BB720): FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
//   (without it VC7.1 expands both push_backs here: 503 v 359 bytes).
// Same-TU bodies (all REAL landed bodies, none a stand-in; everything from the first #define down to the end of ActivateQuests is the
//   landed row rebuild/src/compiled/00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp verbatim, plus engine/NProgressDisplay.h, the
//   CScriptInfoManager declaration and the push_back opt-in this row needs):
//     - CQuestManager::ActivateQuests 0x004B3CE0 (landed), which in turn carries
//     - CBasicString<char>::operator< 0x00429950 (landed) and CQuestManager::LoadResourcesForQuests 0x004B2510 (landed), out of line.
//   The push_back opt-in is this row's; under it the copy of ActivateQuests in this TU calls push_back<CCharString> out of line where
//   retail's 0x004B3CE0 expands it -- that body is not graded here (its own row is), it only supplies what c2 knows about `quests`.
// Layout: engine/CQuestManager.h (FABLE_RETAIL_CQUESTMANAGER + _LOADGAMESTATE + _ACTIVEQUEST + _SAVEGAMESTATE + _ACTIVATEMULTIPLEQUESTS:
//   nested CActivateQuestInfo and its inline constructor, ScriptInfoManager +0x78; + _ACTIVATEQUESTS for the same-TU body);
//   engine/CScriptInfo.h (FABLE_RETAIL_CSCRIPTINFO_ACCESSORS); engine/CArray.h. No header overlay needed (all opt-ins are in the repo
//   headers). CScriptInfoManager, CCodeSectionManager and CTCDayOrNightOnlySupport have no shared header (declarations only here).
//   Descriptive (made-up) name: g_QuestScriptsEnabled 0x01375454 (as in the landed ActivateQuests row; found in no PDB).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
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
#include "engine/NProgressDisplay.h"

class CScriptInfoManager {
public:
    CScriptInfo* GetPScriptInfo(const CCharString& name);   // 0x00CB5AD0
};

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

// 0x004B3CE0 CQuestManager::ActivateQuests (landed: rebuild/src/compiled/00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp), defined ahead
// of ActivateMultipleQuests as in the retail source file: its real landed body, verbatim.
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

bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool game_script, bool load_resources)
{
    CArray<CActivateQuestInfo> quests;
    bool activated = false;
    for (unsigned int i = 0; i < quest_names.size(); ++i) {
        {
            CCharString text("QuestManager: Activate Quest", -1);
            NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
        }
        const CCharString& name = quest_names[i];
        if (IsQuestRegistered(name)) {
            const CScriptInfo* info = ScriptInfoManager->GetPScriptInfo(name);
            if (info) {
                quests.push_back(CActivateQuestInfo(name, info, game_script, load_resources));
                activated = true;
            }
            else {
                quests.push_back(CActivateQuestInfo(name, 0, game_script, load_resources));
                activated = true;
            }
        }
    }
    ActivateQuests(quests);
    return activated;
}