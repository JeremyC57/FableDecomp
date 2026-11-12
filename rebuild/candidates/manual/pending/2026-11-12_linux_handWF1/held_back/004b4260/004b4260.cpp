// CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool game_script, bool load_resources) -- show the
// "QuestManager: Activate Quest" progress text for every name, queue each REGISTERED quest with its script info (or none) and the two
// flags, activate the queued quests together; true when at least one name was registered.
// Leaf: ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B4260, 359 bytes, __thiscall, `ret 0Ch`, bool in al. No flags (speed TU, no EH state).
// Identity: Ego_r.pdb / FableWin.pdb ?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z. The second argument
//   lands in CActivateQuestInfo::GameScript (+8), the third in LoadResources (+9) (CGameScriptInterface::ActivateMultipleQuests
//   0x00892EA0 passes (1, 1), ...WithoutLoadingResources 0x00892EE0 passes (1, 0)); the parameter names are descriptive.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("QuestManager: Activate Quest" 0x01239440), NProgressDisplay::DisplayProgress
//   0x009E9F40 (__fastcall), ~CCharString 0x0099EAE0, CQuestManager::IsQuestRegistered 0x004B00C0, CScriptInfoManager::GetPScriptInfo
//   0x00CB5AD0 (ScriptInfoManager +0x78), CCharString copy ctor 0x0099EC30 (the inline CActivateQuestInfo constructor),
//   std::vector<CQuestManager::CActivateQuestInfo>::push_back 0x004BB720 (out of line), CQuestManager::ActivateQuests 0x004B3CE0,
//   free 0x00BFEA14 (the inline ~vector: ~CCharString over [start, finish) at stride 0xC, then the storage).
// Shape: (1) the progress text is a NAMED block-scoped CCharString (constructed before DisplayProgress's arguments are pushed; VC7.1
//   homes it in the dead quest_names argument slot); (2) `activated = true` is written in BOTH arms of the script-info test (the arms
//   differ only in the PScriptInfo argument, two distinct 12-byte temporaries; VC7.1 merges the stores behind the shared ~CCharString);
//   (3) the private ActivateQuests is defined EARLIER in the retail source file (0x004B3CE0 < 0x004B4260): with its body visible VC7.1
//   knows the local array's address is not kept, which is what puts `activated` in bl, `this` in edi, quest_names in ebp, leaves the
//   index in the frame and keeps the array start in ebp across the destructor loop. A declaration alone gives 349 v 359 bytes.
// Same-TU body: CQuestManager::ActivateQuests 0x004B3CE0 below is a reconstruction of its body from the retail disassembly (not a
//   stand-in, but UNPROVEN: never graded against 0x004B3CE0, no landed row backs it, a verify attempt to grade it ended in ERROR; it is
//   not claimed to be byte-exact -- row 0x004B3CE0 stays open and whoever lands it must regrade this row with the landed body): first pass = resources (the quests that are neither
//   active nor already resource-loaded: remember + LoadResourcesForQuests, and load the quest's code section when one is registered
//   under the quest name), second pass = one CActiveQuest per inactive quest (with a script when the quest has a script info and
//   scripts are enabled: master data alloc/Init/LoadMasterScriptDataFromGameState or GetMasterScriptDataForScript, AllocFunc, Activate,
//   the persisted script text, MESSAGE_EVENT_QUEST_ACCEPTED), pushed on ActiveQuests and dropped from the resource-loaded set.
//   Its callees: IsQuestActive 0x004AF610, set<CCharString> _M_find 0x004B9370 / insert_unique 0x004AC380 / erase 0x004BB270 (and the
//   expanded lower/upper bound 0x004B9D00 / 0x004B9D50 + erase range 0x004B9C10), vector<CCharString>::_M_insert_overflow 0x00433530,
//   CTBaseSingleton<CCodeSectionManager>::Get expanded (instance 0x013CAA68, ctor 0x009F0320, Reset 0x00413590),
//   CCodeSectionManager 0x009F0570 / GetHandleFromLongName 0x009F0500 / LoadSection 0x009F02C0, LoadResourcesForQuests 0x004B2510,
//   CVectorMap LowerBound 0x004B97D0, CBasicString<char>::operator< 0x00429950, LoadMasterScriptDataFromGameState 0x004B3760,
//   GetMasterScriptDataForScript 0x004AFA10, CActiveQuest ctor 0x004B0310, CCountedPointer<CActiveQuest>::DeleteData 0x004BAEF0,
//   CScriptBase::Activate 0x00CB7900 / LoadGameState 0x00CB8690, GetIndexFromRegisteredQuest 0x004AF740,
//   CMessageEventManager::AddMessage 0x00687540, operator new 0x00BFEA1A, malloc 0x00BFEA0E, operator delete 0x00BFE9BC.
// Layout: engine/CQuestManager.h (FABLE_RETAIL_CQUESTMANAGER + _LOADGAMESTATE + _ACTIVEQUEST + _SAVEGAMESTATE, overlay opt-in
//   _ACTIVATEMULTIPLEQUESTS: nested CActivateQuestInfo, ScriptInfoManager +0x78, CurrentQuestID +0x84,
//   QuestsWithResourcesLoadedThatAreNotActivated +0x9C, the CActiveQuest constructors, the typed CRuntimeQuestPersistence maps);
//   engine/CScriptInfo.h overlay opt-in FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC. CScriptInfoManager and CCodeSectionManager have no
//   shared header (declarations only here). Descriptive (made-up) names: g_QuestScriptsEnabled 0x01375454 (one reader, this TU).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CWorld.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptBase.h"
#include "engine/CScriptDataBase.h"
#include "engine/CMessageEventManager.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CQuestManager.h"
#include "engine/NProgressDisplay.h"

class CScriptInfoManager {
public:
    CScriptInfo* GetPScriptInfo(const CCharString& name);   // 0x00CB5AD0
};
class CCodeSectionManager : public CTBaseSingleton<CCodeSectionManager> {   // 0x2c bytes; the instance pointer is 0x013CAA68
public:
    CCodeSectionManager();                                    // 0x009F0320
    bool IsSectionRegisteredLongName(CCharString long_name);  // 0x009F0570
    unsigned long GetHandleFromLongName(CCharString long_name);   // 0x009F0500
    void LoadSection(unsigned long handle);                   // 0x009F02C0
    // ?LoadSectionLongName@CCodeSectionManager@@QAEXVCCharString@@@Z: landed out-of-line COMDAT 0x004AF000
    // (rebuild/src/compiled/00/4a/CCodeSectionManager_LoadSectionLongName_004af000.cpp); expanded inline in ActivateQuests 0x004B3DE3..0x004B3E0D.
    void LoadSectionLongName(CCharString longName) { LoadSection(GetHandleFromLongName(longName)); }
private:
    unsigned char _pad_0x04[0x2c - 0x04];
};
extern bool g_QuestScriptsEnabled;                            // 0x01375454 (descriptive)

// 0x004B3CE0 (real body, see the header comment; defined ahead of ActivateMultipleQuests as in the retail source file).
void CQuestManager::ActivateQuests(const CArray<CActivateQuestInfo>& quests)
{
    CArray<CCharString> quests_to_load_resources_for;
    for (unsigned int i = 0; i < quests.size(); ++i) {
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
    }
    if (!quests_to_load_resources_for.empty())
        LoadResourcesForQuests(quests_to_load_resources_for);
    for (unsigned int j = 0; j < quests.size(); ++j) {
        const CActivateQuestInfo& info = quests[j];
        if (IsQuestActive(info.QuestName))
            continue;
        const CCharString* persist_script = RuntimeQuestPersistence.GetQuestScriptPersistScript(info.QuestName);
        if (info.PScriptInfo && g_QuestScriptsEnabled) {
            long id = CurrentQuestID++;
            CScriptDataBase* script_data = 0;
            CScriptDataBase* master_data;
            if (info.PScriptInfo->MasterScript) {
                script_data = info.PScriptInfo->MasterScriptDataAlloc();
                script_data->Init();
                LoadMasterScriptDataFromGameState(*script_data, info.QuestName);
                master_data = script_data;
            }
            else
                master_data = GetMasterScriptDataForScript(*info.PScriptInfo);
            CScriptBase* script = info.PScriptInfo->AllocFunc(master_data, (CGameScriptInterfaceBase*)&World->GetGameScriptInterface());
            CCountedPointer<CActiveQuest> quest(new CActiveQuest(id, info.QuestName, *info.PScriptInfo, script, script_data, info.LoadResources));
            ActiveQuests.push_back(quest);
            QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
            script->Activate();
            if (persist_script)
                quest->LoadFromGameState(*persist_script);
            World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_ACCEPTED, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f,
                                                       GetIndexFromRegisteredQuest(info.QuestName));
        }
        else {
            long id = CurrentQuestID++;
            CCountedPointer<CActiveQuest> quest(new CActiveQuest(id, info.QuestName, info.LoadResources));
            ActiveQuests.push_back(quest);
            QuestsWithResourcesLoadedThatAreNotActivated.erase(info.QuestName);
            if (persist_script)
                quest->LoadFromGameState(*persist_script);
        }
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
