// Behaviour test for CQuestManager::Update (0x004B4490): the main player's id is published, the thing manager is
// updated, pending deactivations run only for active quests and the pending list is emptied (names destroyed, nodes
// freed, head reset), every active quest with a script gets one frame with itself as the running quest, and the quest
// cards are re-laid only when the living hero's map differs from the remembered one.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0;
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public:
    const char* PStringData;
    CCharString(const char* t) : PStringData(t) { ++g_live; }
    ~CCharString() { --g_live; }
};
class CThing { public: short MapNumber; unsigned char Dead : 1; };
static CThing* g_heroThing = 0;
class CPlayer { public: long PlayerID; CThing* GetHeroThing() { return g_heroThing; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
class CMainGameComponent { public: CPlayerManager* PlayerManager; };
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
class CGameScriptThingManager { public: void FrameUpdate() { Log("frame"); } };
class CGameScriptInterface { public: long MainPlayerID; };
class CQuestManager;
static CQuestManager* g_qm;
struct CActiveQuest;
class CScriptBase { public: const char* name; void UpdateScript(); };
struct CActiveQuest { long ID; void* PScriptInfo; CScriptBase* PScript; };
struct CQuestRunState { long CurrentQuestID; };
class CWorld { public: CQuestRunState* QuestRunState; };
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct list {
    _List_node<T>* _M_node;
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) { _List_node<T>* tmp = cur; cur = cur->_M_next; tmp->_M_data.~T(); std::free(tmp); }
        _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node;
    }
};
struct CPendingDeactivation { CCharString first; unsigned long second; };
struct CActiveQuestPointer { CActiveQuest* Data; void* Info; };
class CQuestManager { public:
    CGameScriptThingManager* PGameScriptThingManager;
    list<CActiveQuestPointer> ActiveQuests;
    list<CPendingDeactivation> PendingQuestsToDeactivate;
    CWorld* World;
    CGameScriptInterface* GameInterface;
    CActiveQuest* PQuestCurrentlyRunning;
    long HeroMapNumber;
    bool IsQuestActive(const CCharString& name) { return std::strcmp(name.PStringData, "Q_Active") == 0; }
    void DeactivateQuest(const CCharString& name, unsigned long reason) { Log("deact"); Log(name.PStringData); if (reason != 3) Log("reason?"); }
    void ResetPositionOfAllQuestCards() { Log("cards"); }
    void Update();
};
void CScriptBase::UpdateScript() { Log(name); if (g_qm->PQuestCurrentlyRunning == 0) Log("norun"); }
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
    CThing* hero = player->GetHeroThing();
    if (hero != 0 && !hero->Dead && hero->MapNumber != HeroMapNumber) {
        ResetPositionOfAllQuestCards();
        HeroMapNumber = hero->MapNumber;
    }
}
static _List_node<CPendingDeactivation>* Pending(const char* name, _List_node<CPendingDeactivation>* head)
{
    _List_node<CPendingDeactivation>* n = (_List_node<CPendingDeactivation>*)std::malloc(sizeof(*n));
    new (&n->_M_data.first) CCharString(name); n->_M_data.second = 3;
    n->_M_next = head; n->_M_prev = head->_M_prev; head->_M_prev->_M_next = n; head->_M_prev = n;
    return n;
}
int main()
{
    int bad = 0;
    _List_node<CPendingDeactivation>& pendHead = *(_List_node<CPendingDeactivation>*)std::malloc(sizeof(_List_node<CPendingDeactivation>));
    pendHead._M_next = pendHead._M_prev = &pendHead;
    Pending("Q_Active", &pendHead); Pending("Q_Gone", &pendHead);
    CScriptBase s1; s1.name = "s1";
    CActiveQuest q1 = { 11, 0, &s1 }, q2 = { 12, 0, 0 };
    _List_node<CActiveQuestPointer> aHead, a1, a2;
    aHead._M_next = &a1; a1._M_next = &a2; a2._M_next = &aHead; a1._M_data.Data = &q1; a2._M_data.Data = &q2;
    CGameScriptThingManager tm; CGameScriptInterface gi; gi.MainPlayerID = 0; CQuestRunState rs; rs.CurrentQuestID = 0; CWorld w; w.QuestRunState = &rs;
    CQuestManager m; g_qm = &m;
    m.PGameScriptThingManager = &tm; m.ActiveQuests._M_node = &aHead; m.PendingQuestsToDeactivate._M_node = &pendHead;
    m.World = &w; m.GameInterface = &gi; m.PQuestCurrentlyRunning = (CActiveQuest*)1; m.HeroMapNumber = 4;
    CThing hero; hero.MapNumber = 7; hero.Dead = 0; g_heroThing = &hero; g_player.PlayerID = 99;
    g_log[0] = 0;
    m.Update();
    if (std::strcmp(g_log, "frame;deact;Q_Active;s1;cards;") != 0) bad = 1;
    if (gi.MainPlayerID != 99 || rs.CurrentQuestID != 11 || m.PQuestCurrentlyRunning != 0 || m.HeroMapNumber != 7) bad = 1;
    if (pendHead._M_next != &pendHead || pendHead._M_prev != &pendHead || g_live != 0) bad = 1;
    // same map / dead hero / no hero: no card reset
    g_log[0] = 0; m.Update(); if (std::strstr(g_log, "cards")) bad = 1;
    hero.MapNumber = 8; hero.Dead = 1; g_log[0] = 0; m.Update(); if (std::strstr(g_log, "cards") || m.HeroMapNumber != 7) bad = 1;
    g_heroThing = 0; g_log[0] = 0; m.Update(); if (std::strstr(g_log, "cards")) bad = 1;
    if (bad) { std::printf("004b4490_TEST FAIL\n"); return 1; }
    std::printf("004b4490_TEST PASS\n");
    return 0;
}