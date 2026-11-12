// CQuestManager::SaveGameState(CCharString&) -- record every active quest's script persist text (and its master-script data when it
// has one), then append the quest-card types, the persisted active quest cards, the saved quest scripts, the persistent active
// quests, the completed quest cards, the creature-generation-disabled quests, the between-start-and-end-screen quests and the
// pending-screen count to the game-state text.
// Leaf: ?SaveGameState@CQuestManager@@QAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B5EE0, 1170 bytes, `ret 4`.
// Callees (Ego_r.pdb / FableWin.pdb spellings): CScriptBase::SaveGameState 0x00CB8820,
//   NQuestManager::CRuntimeQuestPersistence::SetQuestScriptPersistScript 0x004B4E30 / SaveToGameStateScript 0x004AF450,
//   CQuestManager::SaveMasterScriptDataToGameState 0x004B5030, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CThing::IsGameStatePersisted 0x004C8280, CThingManager::RemapUIDForSerialisation 0x0051E210 (CWorld slot 3 GetThingManager),
//   CCharString() 0x0099E4B0 / ~ 0x0099EAE0 / += const char* 0x0099F100 / += CCharString 0x0099F0A0, GFIntToCharString 0x0099F830,
//   GFUHUGEToCharString 0x0099F930, operator+ (const char*, CCharString) 0x0099F690 / (CCharString, const char*) 0x0099F600 /
//   (CCharString, CCharString) 0x0099F570 (all __fastcall).
// Layout: engine/CQuestManager.h (FABLE_RETAIL_CQUESTMANAGER + _LOADGAMESTATE + _ACTIVEQUEST, overlay _SAVEGAMESTATE for the typed
//   ActiveQuests list, the nested CActiveQuest's members and the two persistence members). CQuestCard is the LOADGAMESTATE model
//   (DefName, ScriptName, Flag, FeatIndex; its bool name is descriptive there).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_SAVEGAMESTATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CScriptInfo.h"
#include "engine/CScriptBase.h"
#include "engine/CQuestManager.h"

CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830
CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value);         // 0x0099F930

void CQuestManager::SaveGameState(CCharString& text)
{
    for (std::list<CCountedPointer<CActiveQuest> >::iterator it = ActiveQuests.begin(); it != ActiveQuests.end(); ++it) {
        CActiveQuest* quest = (*it).Data;
        CCharString script;
        if (quest->PScript.Data)
            quest->PScript->SaveGameState(script);
        RuntimeQuestPersistence.SetQuestScriptPersistScript(quest->QuestName, script);
        if (quest->PScript.Data && quest->PScriptInfo->MasterScript && quest->PScriptData.Data)
            SaveMasterScriptDataToGameState(*quest->PScriptData, quest->QuestName);
    }
    text += "\r\nSTART_QUEST_CARDS\r\n";
    for (std::list<CQuestCard>::iterator card = AvailableQuestCardTypes.begin(); card != AvailableQuestCardTypes.end(); ++card) {
        if ((*card).FeatIndex > -1)
            text += "OBJECT_FEAT_CARD_DUMMY, \"" + (*card).DefName + "\", \"" + (*card).ScriptName + "\", " + GFIntToCharString((*card).FeatIndex) + "\r\n";
        else
            text += (*card).DefName + ", " + (*card).ScriptName + ", " + GFIntToCharString((*card).Flag ? 1 : 0) + "\r\n";
    }
    text += "END_QUEST_CARDS\r\n";
    text += "\r\nSTART_ACTIVE_QUEST_CARDS\r\n";
    for (std::list<CIntelligentPointer<CThing> >::iterator it = ActiveQuestCards.begin(); it != ActiveQuestCards.end(); ++it) {
        CThing* thing = (*it).Get();
        if (thing && !thing->IsDead() && thing->IsGameStatePersisted()) {
            unsigned __int64 uid = World->GetThingManager().RemapUIDForSerialisation(false, true, 0, thing->GetUniqueID());
            text += GFUHUGEToCharString(uid) + "\r\n";
        }
    }
    text += "END_ACTIVE_QUEST_CARDS\r\n\r\n";
    text += "START_SAVED_QUESTS\r\n";
    RuntimeQuestPersistence.SaveToGameStateScript(text);
    text += "END_SAVED_QUESTS\r\n\r\n";
    text += "START_ACTIVE_QUESTS\r\n";
    for (std::list<CCountedPointer<CActiveQuest> >::iterator it = ActiveQuests.begin(); it != ActiveQuests.end(); ++it) {
        if ((*it)->Persistent)
            text += (*it)->QuestName + "\r\n";
    }
    text += "END_ACTIVE_QUESTS\r\n\r\n";
    text += "START_COMPLETED_QUESTS\r\n";
    for (std::list<CIntelligentPointer<CThing> >::iterator it = CompletedQuestCards.begin(); it != CompletedQuestCards.end(); ++it) {
        CThing* thing = (*it).Get();
        if (thing && !thing->IsDead()) {
            unsigned __int64 uid = World->GetThingManager().RemapUIDForSerialisation(false, true, 0, thing->GetUniqueID());
            text += GFUHUGEToCharString(uid) + "\r\n";
        }
    }
    text += "END_COMPLETED_QUESTS\r\n\r\n";
    text += "START_CG_DISABLED_QUESTS\r\n";
    for (std::vector<CCharString>::iterator it = CreatureGenerationDisabledQuests.begin(); it != CreatureGenerationDisabledQuests.end(); ++it)
        text += *it + "\r\n";
    text += "END_CG_DISABLED_QUESTS\r\n\r\n";
    text += "BETWEEN_START_AND_END_SCREEN_QUESTS\r\n";
    for (std::vector<CCharString>::iterator it = BetweenStartAndEndScreenQuests.begin(); it != BetweenStartAndEndScreenQuests.end(); ++it)
        text += *it + "\r\n";
    text += "END_BETWEEN_START_AND_END_SCREEN_QUESTS\r\n\r\n";
    text += "NumberOfQuestsBetweenStartAndFinishScreens " + GFIntToCharString(NumberOfQuestsBetweenStartAndFinishScreens) + "\r\n";
}