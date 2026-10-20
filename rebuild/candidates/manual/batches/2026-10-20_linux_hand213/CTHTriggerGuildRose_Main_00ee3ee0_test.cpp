// Behaviour test for CTH_Trigger_Guild_Rose::Main (0x00EE3EE0): the thread filters on its own thing being
// alive, updates, and -- past gameflow position 200 with the hero within 20 units and the thread still
// running -- plays the rose-quest cutscene, fails DUMMY_QUEST_2 and releases its thing; every other state
// leaves the game untouched after the mark.
#include <cstdio>
#include <cstring>
struct CScriptThing { long id; long a, b; CScriptThing(long i) : id(i), a(0), b(0) {} CScriptThing(const CScriptThing& o) : id(o.id), a(o.a), b(o.b) {} ~CScriptThing() { id = -1; } };
static int g_strings = 0;
class CCharString {
public:
    char* PStringData;
    CCharString(const char* text, long length = -1) { ++g_strings; PStringData = new char[std::strlen(text) + 1]; std::strcpy(PStringData, text); (void)length; }
    CCharString(const CCharString& other) { ++g_strings; PStringData = new char[std::strlen(other.PStringData) + 1]; std::strcpy(PStringData, other.PStringData); }
    ~CCharString() { --g_strings; delete[] PStringData; }
};
static int g_updates = 0, g_marks = 0, g_releases = 0, g_fails = 0, g_cutscenes = 0, g_predicates = 0;
static long g_markedId = 0; static bool g_markFlag = false; static long g_releasedId = 0; static long g_releaseA = 1; static bool g_releaseB = false;
static char g_failedQuest[64]; static char g_cutsceneName[64]; static long g_predicateId = 0;
static bool g_terminating = false; static int g_terminateAfterChecks = 0; static bool g_near = true; static long g_heroId = 77;
struct CGameScriptInterface {
    virtual void Update() { ++g_updates; }
    virtual const CScriptThing& GetHero_slot70() { static CScriptThing hero(g_heroId); return hero; }
    virtual void ReleaseThing_slot108(const CScriptThing& thing, long a, bool b) { ++g_releases; g_releasedId = thing.id; g_releaseA = a; g_releaseB = b; }
    virtual void SetQuestAsFailed_slot293(const CCharString& questName) { ++g_fails; std::strcpy(g_failedQuest, questName.PStringData); }
    virtual void MarkThing_slot477(const CScriptThing& thing, bool flag) { ++g_marks; g_markedId = thing.id; g_markFlag = flag; }
};
static long g_distHeroId = 0; static float g_distance = 0;
bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance) { g_distHeroId = b.id; g_distance = distance; (void)a; return g_near; }
struct CIsThingAlive { virtual void v00() {} CScriptThing m_thing; CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { } ~CIsThingAlive() { } };
struct CQ_SunnyvaleMasterData { unsigned char _pad00[4]; long PostSavePosition; };
struct CV_TravellingHeroesScript { void OnRoseQuestCutscene_00ee2db0(CCharString name) { ++g_cutscenes; std::strcpy(g_cutsceneName, name.PStringData); } };
class CTH_Trigger_Guild_Rose {
public:
    void*                     __vftable;
    CGameScriptInterface*     Game;
    CScriptThing              Me;
    CV_TravellingHeroesScript* ParentClass;
    CQ_SunnyvaleMasterData*   PMasterData;
    CTH_Trigger_Guild_Rose() : Me(5) {}
    void SetPredicate(CIsThingAlive& filter) { ++g_predicates; g_predicateId = filter.m_thing.id; }
    bool IsActiveThreadTerminating() { if (g_terminateAfterChecks > 0 && --g_terminateAfterChecks == 0) g_terminating = true; return g_terminating; }
    void Main();
};
void CTH_Trigger_Guild_Rose::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->Update();
    if (IsActiveThreadTerminating())
        return;
    Game->MarkThing_slot477(Me, true);
    if (PMasterData->PostSavePosition > 200) {
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero_slot70(), 20.0f)) {
            if (!IsActiveThreadTerminating()) {
                ParentClass->OnRoseQuestCutscene_00ee2db0(CCharString("CS_TRAVHERO_ROSEQUEST", -1));
                {
                    CCharString questName("DUMMY_QUEST_2", -1);
                    Game->SetQuestAsFailed_slot293(questName);
                }
                Game->ReleaseThing_slot108(Me, 0, true);
            }
        }
    }
}
static int fail(const char* why) { std::printf("00ee3ee0_TEST FAIL: %s\n", why); return 1; }
static void reset() { g_updates = g_marks = g_releases = g_fails = g_cutscenes = g_predicates = 0; g_terminating = false; g_terminateAfterChecks = 0; g_near = true; }
int main()
{
    static CGameScriptInterface game; static CV_TravellingHeroesScript parent; static CQ_SunnyvaleMasterData master;
    CTH_Trigger_Guild_Rose t; t.Game = &game; t.ParentClass = &parent; t.PMasterData = &master;
    // 1. full path
    master.PostSavePosition = 201; reset();
    t.Main();
    if (g_predicates != 1 || g_predicateId != 5 || g_updates != 1 || g_marks != 1 || g_markedId != 5 || !g_markFlag) return fail("predicate/update/mark");
    if (g_distHeroId != 77 || g_distance != 20.0f) return fail("distance query");
    if (g_cutscenes != 1 || std::strcmp(g_cutsceneName, "CS_TRAVHERO_ROSEQUEST") || g_fails != 1 || std::strcmp(g_failedQuest, "DUMMY_QUEST_2")) return fail("cutscene / quest failure");
    if (g_releases != 1 || g_releasedId != 5 || g_releaseA != 0 || !g_releaseB) return fail("release");
    if (g_strings != 0) return fail("string leak");
    // 2. terminating right after the update: nothing more
    reset(); g_terminating = true; t.Main();
    if (g_updates != 1 || g_marks != 0 || g_cutscenes != 0 || g_releases != 0) return fail("early termination");
    // 3. gameflow position not past 200 (boundary 200 itself)
    reset(); master.PostSavePosition = 200; t.Main();
    if (g_marks != 1 || g_cutscenes != 0 || g_fails != 0 || g_releases != 0) return fail("position boundary");
    // 4. hero too far
    reset(); master.PostSavePosition = 300; g_near = false; t.Main();
    if (g_marks != 1 || g_cutscenes != 0 || g_releases != 0) return fail("hero far");
    // 5. terminates on the second check (after the distance test): no cutscene
    reset(); g_terminateAfterChecks = 2; t.Main();
    if (g_marks != 1 || g_cutscenes != 0 || g_fails != 0 || g_releases != 0 || g_strings != 0) return fail("late termination");
    std::printf("00ee3ee0_TEST PASS\n");
    return 0;
}
