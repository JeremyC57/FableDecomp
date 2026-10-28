// CQuestManager::Update() -- per-frame quest update: publish the main player's id to the script interface, update the script thing manager, run the pending quest deactivations and clear that list, run one frame of every active quest's script, and re-lay the quest cards when the hero changed map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B4490, length 254 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (through g_pMainGameComponent 0x013B86A0 +0x1c),
// CGameScriptThingManager::FrameUpdate 0x007047D0, CQuestManager::IsQuestActive(const CCharString&) 0x004AF610
// (propagated label "CQuest_Array_FindByName"), CQuestManager::DeactivateQuest(const CCharString&, unsigned long)
// 0x004B43D0, ~CCharString 0x0099EAE0 and free 0x00BFEA14 (the inline list clear), CScriptBase::UpdateScript
// 0x00CB8220 (propagated label "_Dest_val<...>"), CPlayer::GetPControlledCreature 0x00487DC0, ResetPositionOfAllQuestCards
// 0x004AFCA0.
// Layout (retail, Ego_r.pdb with 12-byte vectors and 4-byte lists): PGameScriptThingManager +0x04, ActiveQuests
// (list<CCountedPointer<CActiveQuest>>, head pointer) +0x38, PendingQuestsToDeactivate (list<pair<CCharString, unsigned
// long>>) +0x3c, World +0x7c, GameInterface +0x80, PQuestCurrentlyRunning +0x88, HeroMapNumber +0x90. CActiveQuest: ID
// +0, PScript +8 (the landed ctor 0x004B0310). CThing: flag byte +0x91 (bit 0 dead), map number (short) +0x8e.
// Local model: engine/CQuestManager.h carries the PDB offsets (ActiveQuests +0x44 ... HeroMapNumber +0xec), not retail's.
// Shape: list walks re-read the head member each iteration; the clear is STLport's `_List_base::clear` inlined
// (tmp/next, destroy, deallocate, then head->next = head->prev = head); one zero register (ebx) serves the script test,
// the running-quest reset and the hero null test.
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

class CCharString { public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};

class CThingPlayerCreature;
class CPlayer { public:
    unsigned char _pad0[0x28];
    long          PlayerID;                            // +0x28
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                          // 0x00449970
};
class CMainGameComponent { public:
    unsigned char   _pad0[0x1c];
    CPlayerManager* PlayerManager;                     // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

class CThing { public:
    unsigned char _pad0[0x8e];
    short         MapNumber;                           // +0x8e
    unsigned char _pad90;
    unsigned char Dead : 1;                            // +0x91 bit 0
};
class CThingPlayerCreature : public CThing { };

class CGameScriptThingManager { public:
    void FrameUpdate();                                // 0x007047D0
};
class CGameScriptInterface { public:
    unsigned char _pad0[0x1c];
    long          MainPlayerID;                        // +0x1c
};
class CScriptBase { public:
    void UpdateScript();                               // 0x00CB8220
};
struct CActiveQuest {
    long         ID;                                   // +0x00
    void*        PScriptInfo;                          // +0x04
    CScriptBase* PScript;                              // +0x08 (CCountedPointer<CScriptBase>::Data)
};
struct CQuestRunState {                                // World+0x38
    unsigned char _pad0[0x28];
    long          CurrentQuestID;                      // +0x28
};
class CWorld { public:
    unsigned char   _pad0[0x38];
    CQuestRunState* QuestRunState;                     // +0x38
};

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct list {                       // STLport list: the head node pointer (4 bytes in retail)
    _List_node<T>* _M_node;
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            tmp->_M_data.~T();
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
struct CPendingDeactivation {                          // pair<CCharString, unsigned long>
    CCharString   first;
    unsigned long second;
};
struct CActiveQuestPointer {                           // CCountedPointer<CActiveQuest>
    CActiveQuest* Data;
    void*         Info;
};

class CQuestManager { public:
    void*                    __vftable;                // +0x00
    CGameScriptThingManager* PGameScriptThingManager;  // +0x04
    void*                    PGameScriptThingManager_Info;   // +0x08
    unsigned char            _pad0c[0x2c];             // CRuntimeQuestPersistence, RegisteredQuestNames
    list<CActiveQuestPointer>  ActiveQuests;           // +0x38
    list<CPendingDeactivation> PendingQuestsToDeactivate;   // +0x3c
    unsigned char            _pad40[0x3c];
    CWorld*                  World;                    // +0x7c
    CGameScriptInterface*    GameInterface;            // +0x80
    long                     CurrentQuestID;           // +0x84
    CActiveQuest*            PQuestCurrentlyRunning;   // +0x88
    unsigned char            _pad8c[4];                // DayActive .. QuestCardsNeedUpdating
    long                     HeroMapNumber;            // +0x90
    bool IsQuestActive(const CCharString& name);       // 0x004AF610
    void DeactivateQuest(const CCharString& name, unsigned long reason);   // 0x004B43D0
    void ResetPositionOfAllQuestCards();               // 0x004AFCA0
    void Update();
};

void CQuestManager::Update()
{
    CPlayer* player = g_pMainGameComponent->PlayerManager->GetMainPlayer();
    GameInterface->MainPlayerID = player->PlayerID;
    PGameScriptThingManager->FrameUpdate();
    for (_List_node<CPendingDeactivation>* n = PendingQuestsToDeactivate._M_node->_M_next; n != PendingQuestsToDeactivate._M_node; n = n->_M_next) {
        if (IsQuestActive(n->_M_data.first))
            DeactivateQuest(n->_M_data.first, n->_M_data.second);
    }
    PendingQuestsToDeactivate.clear();
    for (_List_node<CActiveQuestPointer>* q = ActiveQuests._M_node->_M_next; q != ActiveQuests._M_node; q = q->_M_next) {
        CActiveQuest* quest = q->_M_data.Data;
        if (quest->PScript != 0) {
            World->QuestRunState->CurrentQuestID = quest->ID;
            PQuestCurrentlyRunning = quest;
            quest->PScript->UpdateScript();
            PQuestCurrentlyRunning = 0;
        }
    }
    CThing* hero = player->GetPControlledCreature();
    if (hero != 0 && !hero->Dead && hero->MapNumber != HeroMapNumber) {
        ResetPositionOfAllQuestCards();
        HeroMapNumber = hero->MapNumber;
    }
}