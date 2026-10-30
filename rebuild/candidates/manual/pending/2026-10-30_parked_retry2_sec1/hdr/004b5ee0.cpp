// CQuestManager::SaveGameState(CCharString&) -- record every active quest's script persist text (and its master-script data when it has one), then append the quest-card types, the persisted active quest cards, the saved quest scripts, the persistent active quests, the completed quest cards, the creature-generation-disabled quests, the between-start-and-end-screen quests and the pending-screen count to the game-state text
// Grade: DIFFER(1170v1170) -- same length and shape; only the callee-saved register assignment differs (retail this=edi,
//   loop nodes ebp/ebx; ours this=ebp). The card-loop alias, the bool->long ternary and the UID statement split are applied.
// Leaf: SaveGameState
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B5EE0, 1170 bytes (int3 padded to 1184), `ret 4`.
// Callees: CScriptBase::SaveGameState 0x00CB8820, SetQuestScriptPersistScript 0x004B4E30, SaveMasterScriptDataToGameState
//   0x004B5030, SaveToGameStateScript 0x004AF450, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::IsGameStatePersisted
//   0x004C8280, CThingManager::RemapUIDForSerialisation 0x0051E210 (thing manager from the world's slot 3);
//   CCharString() 0x0099E4B0 / ~ 0x0099EAE0 / += const char* 0x0099F100 / += CCharString 0x0099F0A0; `__fastcall`
//   GFIntToCharString 0x0099F830, GFUInt64ToCharString 0x0099F930, operator+ (const char*, CCharString) 0x0099F690 /
//   (CCharString, const char*) 0x0099F600 / (CCharString, CCharString) 0x0099F570.
// Layout: retail STLport CQuestManager (Ego_r.pdb minus 4-byte list headers / 12-byte vectors: RuntimeQuestPersistence
//   +0xc, ActiveQuests +0x38, CreatureGenerationDisabledQuests +0x40, BetweenStartAndEndScreenQuests +0x4c,
//   AvailableQuestCardTypes +0x58, ActiveQuestCards +0x5c, CompletedQuestCards +0x60, World +0x7c,
//   NumberOfQuestsBetweenStartAndFinishScreens +0x98); CActiveQuest PScriptInfo +4, PScript +8, Persistent +0x24,
//   PScriptData +0x28, QuestName +0x30; CQuestCard {DefName, ScriptName, Replayable, FeatIndex}; CThing Dead bit +0x91.
#include "engine/CCountedPointer.h"
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830
CCharString __fastcall GFUInt64ToCharString(unsigned __int64 value);   // 0x0099F930
CCharString __fastcall operator+(const char* left, const CCharString& right);         // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);         // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);  // 0x0099F570
struct CScriptBase {
    void SaveGameState(CCharString& out);               // 0x00CB8820
};
struct CScriptDataBase;
struct CScriptInfo {
    unsigned char _pad00[0x10];
    bool MasterScript;                                  // +0x10
};

struct CActiveQuest {
    long                             ID;                // +0x00
    CScriptInfo*                     PScriptInfo;       // +0x04
    CCountedPointer<CScriptBase>     PScript;           // +0x08
    unsigned char                    _pad10[0x24 - 0x10];
    bool                             Persistent;        // +0x24
    bool                             LoadedResources;   // +0x25
    CCountedPointer<CScriptDataBase> PScriptData;       // +0x28
    CCharString                      QuestName;         // +0x30
};
struct CQuestCard {
    CCharString DefName;                                // +0x00
    CCharString ScriptName;                             // +0x04
    bool        Replayable;                             // +0x08
    long        FeatIndex;                              // +0x0c
};
struct CThing {
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                               // +0x18
    unsigned char _pad20[0x91 - 0x20];
    unsigned char Dead : 1;                             // +0x91
    bool IsGameStatePersisted();                        // 0x004C8280
};
struct CBaseIntelligentPointer {
    void* Data;
    void* Info;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CThingManager {
    unsigned __int64 RemapUIDForSerialisation(bool a, bool b, unsigned long flags, unsigned __int64 uid);   // 0x0051E210
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();           // slot 3 (+0x0c)
};
template <class T> struct CList {                       // STLport list<T> (4-byte header)
    struct Node { Node* next; Node* prev; T value; };
    Node* m_node;
};
template <class T> struct CVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
namespace NQuestManager {
struct CRuntimeQuestPersistence {
    unsigned char _body[0x20];
    void SetQuestScriptPersistScript(const CCharString& quest, const CCharString& script);   // 0x004B4E30
    void SaveToGameStateScript(CCharString& out);       // 0x004AF450
};
}
struct CQuestManager {
    unsigned char _pad00[0x0c];
    NQuestManager::CRuntimeQuestPersistence RuntimeQuestPersistence;   // +0x0c
    CVector<CCharString> RegisteredQuestNames;          // +0x2c
    CList<CCountedPointer<CActiveQuest> > ActiveQuests; // +0x38
    void* PendingQuestsToDeactivate;                    // +0x3c
    CVector<CCharString> CreatureGenerationDisabledQuests;   // +0x40
    CVector<CCharString> BetweenStartAndEndScreenQuests;     // +0x4c
    CList<CQuestCard> AvailableQuestCardTypes;          // +0x58
    CList<CBaseIntelligentPointer> ActiveQuestCards;    // +0x5c
    CList<CBaseIntelligentPointer> CompletedQuestCards; // +0x60
    unsigned char _pad64[0x7c - 0x64];
    CWorld* World;                                      // +0x7c
    unsigned char _pad80[0x98 - 0x80];
    long NumberOfQuestsBetweenStartAndFinishScreens;    // +0x98
    void SaveMasterScriptDataToGameState(CScriptDataBase* data, const CCharString& questName);   // 0x004B5030
    void SaveGameState(CCharString& out);
};

void CQuestManager::SaveGameState(CCharString& out)
{
    for (CList<CCountedPointer<CActiveQuest> >::Node* n = ActiveQuests.m_node->next; n != ActiveQuests.m_node; n = n->next) {
        CActiveQuest* quest = n->value.Data;
        CCharString script;
        if (quest->PScript.Data)
            quest->PScript.Data->SaveGameState(script);
        RuntimeQuestPersistence.SetQuestScriptPersistScript(quest->QuestName, script);
        if (quest->PScript.Data && quest->PScriptInfo->MasterScript && quest->PScriptData.Data)
            SaveMasterScriptDataToGameState(quest->PScriptData.Data, quest->QuestName);
    }
    out += "\r\nSTART_QUEST_CARDS\r\n";
    for (CList<CQuestCard>::Node* n = AvailableQuestCardTypes.m_node->next; n != AvailableQuestCardTypes.m_node; n = n->next) {
        if (n->value.FeatIndex > -1)
            out += "OBJECT_FEAT_CARD_DUMMY, \"" + n->value.DefName + "\", \"" + n->value.ScriptName + "\", " + GFIntToCharString(n->value.FeatIndex) + "\r\n";
        else
            out += n->value.DefName + ", " + n->value.ScriptName + ", " + GFIntToCharString(n->value.Replayable ? 1 : 0) + "\r\n";
    }
    out += "END_QUEST_CARDS\r\n";
    out += "\r\nSTART_ACTIVE_QUEST_CARDS\r\n";
    for (CList<CBaseIntelligentPointer>::Node* n = ActiveQuestCards.m_node->next; n != ActiveQuestCards.m_node; n = n->next) {
        CThing* thing = n->value.GetPItem();
        if (thing && !thing->Dead && thing->IsGameStatePersisted())
        {
            unsigned __int64 uid = World->GetThingManager()->RemapUIDForSerialisation(false, true, 0, thing->UID);
            out += GFUInt64ToCharString(uid) + "\r\n";
        }
    }
    out += "END_ACTIVE_QUEST_CARDS\r\n\r\n";
    out += "START_SAVED_QUESTS\r\n";
    RuntimeQuestPersistence.SaveToGameStateScript(out);
    out += "END_SAVED_QUESTS\r\n\r\n";
    out += "START_ACTIVE_QUESTS\r\n";
    for (CList<CCountedPointer<CActiveQuest> >::Node* n = ActiveQuests.m_node->next; n != ActiveQuests.m_node; n = n->next) {
        if (n->value.Data->Persistent)
            out += n->value.Data->QuestName + "\r\n";
    }
    out += "END_ACTIVE_QUESTS\r\n\r\n";
    out += "START_COMPLETED_QUESTS\r\n";
    for (CList<CBaseIntelligentPointer>::Node* n = CompletedQuestCards.m_node->next; n != CompletedQuestCards.m_node; n = n->next) {
        CThing* thing = n->value.GetPItem();
        if (thing && !thing->Dead)
        {
            unsigned __int64 uid = World->GetThingManager()->RemapUIDForSerialisation(false, true, 0, thing->UID);
            out += GFUInt64ToCharString(uid) + "\r\n";
        }
    }
    out += "END_COMPLETED_QUESTS\r\n\r\n";
    out += "START_CG_DISABLED_QUESTS\r\n";
    for (CCharString* it = CreatureGenerationDisabledQuests._M_start; it != CreatureGenerationDisabledQuests._M_finish; ++it)
        out += *it + "\r\n";
    out += "END_CG_DISABLED_QUESTS\r\n\r\n";
    out += "BETWEEN_START_AND_END_SCREEN_QUESTS\r\n";
    for (CCharString* it = BetweenStartAndEndScreenQuests._M_start; it != BetweenStartAndEndScreenQuests._M_finish; ++it)
        out += *it + "\r\n";
    out += "END_BETWEEN_START_AND_END_SCREEN_QUESTS\r\n\r\n";
    out += "NumberOfQuestsBetweenStartAndFinishScreens " + GFIntToCharString(NumberOfQuestsBetweenStartAndFinishScreens) + "\r\n";
}
