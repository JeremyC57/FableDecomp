// Behaviour test for NScript::CV_AmbushScamScript::Main (0x00E2B090): bind the three entity scripts, wait to enter Witchwood2
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

void CV_AmbushScamScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AmbushScamScript>("AS_Ambusher", this, &CAS_Ambusher::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AmbushScamScript>("AS_Trickster", this, &CAS_Trickster::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AmbushScamScript>("AS_Buddy", this, &CAS_Buddy::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString region_name;
    CScriptThing entrance_point;
    Game->SetCreatureGeneratorsEnabled("Witchwood2", false);
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        while (!Game->MsgOnRegionLoaded(region_name)) {
            Game->NewScriptFrame();
            do { if (IsActiveThreadTerminating()) return; } while (0);
        }
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (QuestActivated && region_name == "Witchwood2") {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            break;
        }
        entrance_point = Game->GetNearestWithDefName(Game->GetHero(), "REGION_ENTRANCE_POINT");
        if (region_name == "Witchwood2" && entrance_point.GetDataString() == "FromWitchwood1") {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            break;
        }
        Game->NewScriptFrame();
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    if (!QuestActivated) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (Game->GetHeroMoralityCategory() != 2 && Game->GetHeroMoralityCategory() != 1 && Game->GetHeroMoralityCategory() != 0) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            Game->ActivateQuest("V_AmbushScamGood");
            MoralitySwitch = true;
        } else {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            Game->ActivateQuest("V_AmbushScamEvil");
            MoralitySwitch = false;
        }
        QuestActivated = true;
    }
    TricksterApproachProximity = pGlobalScriptDef->AmbushScamTricksterApproachProximityLow;
    AddSpawnedFunction(new CSpawnedFunc<CV_AmbushScamScript>("WatchForQuestFinished", 0, this, &CV_AmbushScamScript::WatchForQuestFinished), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_AmbushScamScript>("WatchForRegionTransition", 0, this, &CV_AmbushScamScript::WatchForRegionTransition), CCharString(""));
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if ((TricksterApproachComplete && AmbushSpringComplete) || QuestFinished)
            break;
        if (!TricksterApproachComplete) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            TricksterApproach();
            TricksterApproachComplete = true;
        }
        if (!AmbushSpringComplete) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            SpringAmbush();
            AmbushSpringComplete = true;
        }
        Game->NewScriptFrame();
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    do { if (IsActiveThreadTerminating()) return; } while (0);
    while (!QuestFinished && (!LeftRegion || TricksterApproachComplete)) {
        Game->NewScriptFrame();
        do { if (IsActiveThreadTerminating()) return; } while (0);
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    if (AmbushSpringComplete) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (HeroAttackOnTricksterAfterAmbush || HeroAttackOnTricksterKillingBlow || TricksterKilledAfterBeingSpared) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            AssignMoralityEtcForEvilEnding();
        } else if (HeroTalkToTrickster) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            AssignMoralityEtcForGoodEnding();
        }
    }
    while (!Game->MsgOnRegionLoaded(region_name) && !LeftRegion) {
        Game->NewScriptFrame();
        do { if (IsActiveThreadTerminating()) return; } while (0);
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    Game->SetCreatureGeneratorsEnabled("Witchwood2", true);
    if (MoralitySwitch) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        Game->DeactivateQuestLater("V_AmbushScamGood", 0);
    } else {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        Game->DeactivateQuestLater("V_AmbushScamEvil", 0);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}

static void Reset(CV_AmbushScamScript& s, GameCalls* g)
{
    std::memset(&s, 0, sizeof(s)); s.Game = g; g_frame = 0; g_log_n = 0; g_termAt = 1000;
    for (int i = 0; i < 16; ++i) g_regionAt[i] = 0;
}
int main()
{
    GameCalls g; CV_AmbushScamScript s;
    // Good path: Witchwood1 at frame 1 (ignored), Witchwood2 from Witchwood1 at frame 3; morality 3 -> good quest; talk ending.
    Reset(s, &g); g_regionAt[1] = "Witchwood1"; g_regionAt[3] = "Witchwood2"; g_entranceData = "FromWitchwood1"; g_morality = 3;
    s.HeroTalkToTrickster = true; g_regionAt[8] = "Witchwood1";
    s.Main();
    CHECK(FindLog("Bind:AS_Ambusher") == 0 && FindLog("Bind:AS_Trickster") == 1 && FindLog("Bind:AS_Buddy") == 2);
    CHECK(FindLog("PostAdd") == 3 && FindLog("Game.PostAdd") == 4 && FindLog("Gen-Witchwood2") == 5);
    CHECK(FindLog("Activate:V_AmbushScamGood") == 6 && s.MoralitySwitch && s.QuestActivated);
    CHECK(s.TricksterApproachProximity == 7.5f);
    CHECK(FindLog("Spawn:WatchForQuestFinished") == 7 && FindLog("Spawn:WatchForRegionTransition") == 8);
    CHECK(FindLog("Approach") == 9 && FindLog("Ambush") == 10 && s.TricksterApproachComplete && s.AmbushSpringComplete);
    CHECK(FindLog("GoodEnding") == 11 && FindLog("EvilEnding") < 0);
    CHECK(FindLog("Gen+Witchwood2") == 12 && FindLog("Deactivate:V_AmbushScamGood") == 13 && FindLog("Deactivate:ActiveQuest") == 14);
    CHECK(g_log_n == 15 && g_frame == 8);
    // Evil path: quest already active, Witchwood2 at frame 0 breaks at once (no re-activation); the after-ambush attack
    // gives the evil ending; LeftRegion ends the last wait.
    Reset(s, &g); g_regionAt[0] = "Witchwood2"; g_entranceData = ""; g_morality = 1; s.QuestActivated = true;
    s.HeroAttackOnTricksterAfterAmbush = true; s.LeftRegion = true;
    s.Main();
    CHECK(FindLog("Activate:V_AmbushScamGood") < 0 && FindLog("Activate:V_AmbushScamEvil") < 0 && !s.MoralitySwitch);
    CHECK(FindLog("EvilEnding") >= 0 && FindLog("GoodEnding") < 0 && FindLog("Deactivate:V_AmbushScamEvil") >= 0);
    // Not activated yet, evil morality: activates the evil quest.
    Reset(s, &g); g_regionAt[0] = "Witchwood2"; g_entranceData = "FromWitchwood1"; g_morality = 1; s.LeftRegion = true;
    s.Main();
    CHECK(FindLog("Activate:V_AmbushScamEvil") >= 0 && !s.MoralitySwitch && FindLog("Deactivate:V_AmbushScamEvil") >= 0);
    // Wrong entrance: keeps waiting until the thread terminates; nothing activated, generators stay off.
    Reset(s, &g); g_regionAt[0] = "Witchwood2"; g_entranceData = "FromOakvale"; g_termAt = 5;
    s.Main();
    CHECK(!s.QuestActivated && FindLog("Gen-Witchwood2") >= 0 && FindLog("Gen+Witchwood2") < 0 && FindLog("Spawn:WatchForQuestFinished") < 0);
    // Terminating straight away: returns after binding and disabling the generators.
    Reset(s, &g); g_termAt = 0;
    s.Main();
    CHECK(g_log_n == 6 && FindLog("Gen-Witchwood2") == 5);
    if (g_fail) { std::printf("00e2b090_TEST FAIL\n"); return 1; }
    std::printf("00e2b090_TEST PASS\n");
    return 0;
}