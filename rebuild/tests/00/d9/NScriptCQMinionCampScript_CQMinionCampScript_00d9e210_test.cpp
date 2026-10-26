// Behaviour test for 0x00D9E210 CQ_MinionCampScript ctor: argument words, timer order, zeroed script things and
// two self-linked list sentinels from malloc.
#include <cstdio>
#include <cstdlib>
static long g_nextTimer = 10;
static int g_mallocs = 0;
static void* test_malloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
struct CGameScriptInterface { virtual long RegisterTimer() { return g_nextTimer++; } };
static CGameScriptInterface g_iface;
static CGameScriptInterface* g_pGameScriptInterface = &g_iface;
struct CTimer { long Index; CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { } };
struct CScriptBase { virtual ~CScriptBase() { } unsigned char _body[0x3c]; CScriptBase() { } };
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game; void* Data;
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CBaseClass { virtual ~CBaseClass() { } CBaseClass() { } };
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass { CCountedPointer<CScriptThing> PImp; CScriptThing() : CBaseClass(), PImp() { } };
struct CRetailList_CScriptThing {
    struct node { node* next; node* prev; unsigned char value[12]; };
    node* head;
    CRetailList_CScriptThing() : head(0)
    {
        node* sentinel = (node*)test_malloc(sizeof(node));
        sentinel->next = sentinel; sentinel->prev = sentinel; head = sentinel;
    }
};
struct CQ_MinionCampScript : CQuestScriptObject {
    bool flags[6]; long NumberOfMinions, MaxNumberOfMinions; float MinP, MaxP;
    CTimer ChantTimer; long ChantTimerBar; bool ChantTimerPaused; long PausedTime;
    CTimer MinionSpawnTimer; long CurrentStage;
    CScriptThing HeroAttackerOne, HeroAttackerTwo;
    CRetailList_CScriptThing LeftSpawnPoints, RightSpawnPoints;
    bool ShouldSpawn; CScriptThing SpawnPointToSpawn; long BriarState;
    CQ_MinionCampScript(void* data, CGameScriptInterface* game) : CQuestScriptObject(data, game) { }
};
int main()
{
    int d = 1;
    CQ_MinionCampScript s(&d, &g_iface);
    if (s.Data != &d || s.Game != &g_iface || s.ChantTimer.Index != 10 || s.MinionSpawnTimer.Index != 11
        || s.HeroAttackerOne.PImp.Data || s.HeroAttackerTwo.PImp.Info || s.SpawnPointToSpawn.PImp.Data
        || g_mallocs != 2 || !s.LeftSpawnPoints.head || s.LeftSpawnPoints.head->next != s.LeftSpawnPoints.head
        || s.RightSpawnPoints.head->prev != s.RightSpawnPoints.head || s.LeftSpawnPoints.head == s.RightSpawnPoints.head) {
        std::printf("00d9e210_TEST FAIL\n");
        return 1;
    }
    std::printf("00d9e210_TEST PASS\n");
    return 0;
}