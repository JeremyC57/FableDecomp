import os
W=os.environ['W']
body=open(W+'/main_body.txt').read().replace('void NScript::CV_AmbushScamScript::Main()','void CV_AmbushScamScript::Main()')
head=r'''// Behaviour test for NScript::CV_AmbushScamScript::Main (0x00E2B090): bind the three entity scripts, wait to enter Witchwood2
// from Witchwood1 (or with the quest already active), activate the good or evil variant by hero morality, run the trickster
// approach and the ambush, award the ending, wait to leave the region and close the quest; every terminate check returns.
// The body below is the landed row's text over mock engine types (script calls are logged).
#include <cstdio>
#include <cstring>
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
static int g_log_n = 0;
static char g_log[64][64];
static void LOG(const char* a, const char* b = "")
{
    if (g_log_n < 64) { std::strncpy(g_log[g_log_n], a, 31); g_log[g_log_n][31] = 0; std::strncat(g_log[g_log_n], b, 31); }
    ++g_log_n;
}
static int FindLog(const char* s)
{
    for (int i = 0; i < g_log_n && i < 64; ++i) if (std::strcmp(g_log[i], s) == 0) return i;
    return -1;
}
class CCharString { public:
    const char* p;
    CCharString() : p("") {}
    CCharString(const char* t, long = -1) : p(t) {}
    bool operator==(const char* t) const { return std::strcmp(p, t) == 0; }
};
static int g_frame = 0;
static int g_termAt = 1000;
static const char* g_regionAt[16];          // region-loaded message per frame (0 = none)
static const char* g_entranceData = "";
class CScriptThing { public:
    const char* data;
    CScriptThing() : data("") {}
    CCharString GetDataString() const { return CCharString(data); }
};
static long g_morality = 0;
struct GameCalls {
    void PostAddScriptedEntities() { LOG("Game.PostAdd"); }
    void SetCreatureGeneratorsEnabled(const CCharString& region, bool on) { LOG(on ? "Gen+" : "Gen-", region.p); }
    bool MsgOnRegionLoaded(CCharString& name) { if (g_frame < 16 && g_regionAt[g_frame]) { name = g_regionAt[g_frame]; return true; } return false; }
    void NewScriptFrame() { ++g_frame; }
    CScriptThing GetHero() { return CScriptThing(); }
    CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString& def) { CScriptThing t; if (def == "REGION_ENTRANCE_POINT") t.data = g_entranceData; return t; }
    long GetHeroMoralityCategory() { return g_morality; }
    void ActivateQuest(const CCharString& q) { LOG("Activate:", q.p); }
    void DeactivateQuestLater(const CCharString& q, long) { LOG("Deactivate:", q.p); }
    CCharString GetActiveQuestName() { return CCharString("ActiveQuest"); }
};
struct CGlobalScriptDef { float AmbushScamTricksterApproachProximityLow; };
static CGlobalScriptDef g_def = { 7.5f };
static CGlobalScriptDef* pGlobalScriptDef = &g_def;
struct CV_AmbushScamScript;
template<class T> struct CEntityScriptBinding {
    typedef int (*AllocFn)();
    CEntityScriptBinding(const char* name, T*, AllocFn, int) { LOG("Bind:", name); }
};
template<class T> struct CSpawnedFunc {
    CCharString Name;
    CSpawnedFunc(const char* name, unsigned long, T*, void (T::*)()) : Name(name) {}
};
struct CV_AmbushScamScript {
    struct CAS_Ambusher { static int Alloc() { return 1; } };
    struct CAS_Trickster { static int Alloc() { return 2; } };
    struct CAS_Buddy { static int Alloc() { return 3; } };
    GameCalls* Game;
    bool QuestFinished, QuestActivated, LeftRegion, MoralitySwitch, TricksterApproachComplete, AmbushSpringComplete;
    bool HeroAttackOnTricksterAfterAmbush, HeroAttackOnTricksterKillingBlow, TricksterKilledAfterBeingSpared, HeroTalkToTrickster;
    float TricksterApproachProximity;
    bool IsActiveThreadTerminating() { return g_frame >= g_termAt; }
    void AddEntityScriptBinding(CEntityScriptBinding<CV_AmbushScamScript>* b) { delete b; }
    void PostAddScriptedEntities() { LOG("PostAdd"); }
    void AddSpawnedFunction(CSpawnedFunc<CV_AmbushScamScript>* f, const CCharString&) { LOG("Spawn:", f->Name.p); delete f; }
    void WatchForQuestFinished() {}
    void WatchForRegionTransition() {}
    void TricksterApproach() { LOG("Approach"); ++g_frame; }
    void SpringAmbush() { LOG("Ambush"); ++g_frame; QuestFinished = true; }
    void AssignMoralityEtcForEvilEnding() { LOG("EvilEnding"); }
    void AssignMoralityEtcForGoodEnding() { LOG("GoodEnding"); }
    void Main();
};

'''
tail=open(W+'/scripts/test_tail.cpp').read()
open(W+'/main_test.cpp','w').write(head+body+tail)
