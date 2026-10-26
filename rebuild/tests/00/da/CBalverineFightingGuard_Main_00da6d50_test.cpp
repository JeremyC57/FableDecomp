// 00da6d50 behaviour test: CBalverineFightingGuard::Main (match.cpp body verbatim, host stand-ins)
// shared host stand-ins for the I-44 behaviour tests (inlined into each test by gen.py)
#include <stdio.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static int g_calls = 0, g_termAt = 0, g_frames = 0;
static void L(const char* s) { g_log += s; g_log += ";"; }
class CCharString { public:
    std::string S;
    CCharString(const char* t, long n = -1) : S(t) { (void)n; }
};
static CCharString operator+(const CCharString& a, const CCharString& b) { CCharString r(""); r.S = a.S + b.S; return r; }
class CRGBColour { public: unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) {} };
// a thing: Id, and how many IsAlive checks it survives
static int g_alive[8];
namespace N_006e7b40 { struct CScriptThing {
    int Id;
    CScriptThing() : Id(0) {}
    explicit CScriptThing(int id) : Id(id) {}
    virtual ~CScriptThing() {}
    virtual bool IsAlive() const { L("alive?"); if (g_alive[Id] > 0) { --g_alive[Id]; return true; } return false; }
    bool MsgIsHitBy(const char* by) const { std::string b(by); return (g_frames == 2 && Id == 2 && b == "") || (g_frames == 3 && Id == 3 && b == "CorneredGuard"); }
}; } using N_006e7b40::CScriptThing;
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};

static int g_dist = 0, g_missionAt = 0;
class CScriptDef { public: long GuardTalkDistance; long GuardRescueMorality; };
static CScriptDef g_def = { 7, 25 };
static CScriptDef* g_pScriptDef = &g_def;
struct CQ_MinionClifftopChaseScript;
static CQ_MinionClifftopChaseScript* g_parent;
static int g_screenAt = 0;
class CGameScriptInterface { public:
    CScriptThing Hero;
    CGameScriptInterface() : Hero(1) {}
    void NewScriptFrame() const;
    const CScriptThing& GetHero() const { return Hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing& t, const CCharString& n) const { CHECK(t.Id == 1); CHECK(n.S == "MinionAttackingGuard"); L("nearest"); return CScriptThing(3); }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { CHECK(t.Id == 2 && !a && b); L("remove"); }
    void GiveHeroMorality(long m) const { CHECK(m == 25); L("morality"); }
    void EntityFollowThing(const CScriptThing& t, const CScriptThing& g, float d, bool a) const { CHECK(t.Id == 2 && g.Id == 1 && d == 3.0f && a); L("follow"); }
    void DisplayQuestInfo(bool s) const { CHECK(s); L("questinfo"); }
    long AddQuestInfoBarHealth(const CScriptThing& t, const CRGBColour& c, const CCharString& i, float a) const { CHECK(t.Id == 2 && c.G == 255 && c.R == 0 && i.S == "HUD_QUEST_ICON_GUARD" && a == 1.0f); L("bar"); return 77; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) const { CHECK(t.Id == 2 && !a && !b); L("conv"); return 5; }
    void AddLineToConversation(long c, const CCharString& line, bool a, const CScriptThing& s, const CScriptThing& l) const { CHECK(c == 5 && !a && s.Id == 2 && l.Id == 0); L(line.S.c_str()); }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& g) const { char b[16]; sprintf(b, "enemy%d>%d", t.Id, g.Id); L(b); }
    void SetEntityAsRegionFollowing(const CScriptThing& t, const CScriptThing& l, bool f) const { CHECK(t.Id == 1 && l.Id == 2 && f); L("region"); }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d) { CHECK(a.Id == 2 && b.Id == 1 && d == 7.0f); return g_frames >= g_dist; }
class CSpawnedFuncBase { public: std::string Name; CSpawnedFuncBase(const CCharString& n, unsigned long id) : Name(n.S) { (void)id; } virtual ~CSpawnedFuncBase() {} };
template <class T, class P1> struct CSpawnedFunc1 : CSpawnedFuncBase {
    void (T::*Func)(P1); T* PClass; P1 Par1;
    CSpawnedFunc1(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1), const P1 par1) : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) {}
};
struct CQ_MinionClifftopChaseScript {
    bool MissionFailed, MissionSucceeded, QuestStartScreened;
    std::string Spawned; int SpawnedPar;
    void GuardChatter(CScriptThing guard) { (void)guard; }
    void AddSpawnedFunction(CSpawnedFuncBase* f, const CCharString& region) {
        CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>* s = static_cast<CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>*>(f);
        CHECK(region.S == "" && s->PClass == this && s->Func == &CQ_MinionClifftopChaseScript::GuardChatter);
        Spawned = f->Name; SpawnedPar = s->Par1.Id; L("spawn"); delete f;
    }
};
void CGameScriptInterface::NewScriptFrame() const {
    ++g_frames; L("|");
    if (g_frames >= g_screenAt) g_parent->QuestStartScreened = true;
    if (g_missionAt && g_frames >= g_missionAt) g_parent->MissionSucceeded = true;
}
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} };
class CBalverineFightingGuard {
public:
    CGameScriptInterface* Game; CScriptThing Me; CQ_MinionClifftopChaseScript* ParentClass; void* PMasterData;
    bool ExplainedAboutSummoning; long HealthBarID;
    void RegisterFilter(CIsThingAlive& f) { CHECK(f.m_thing.Id == 2); L("filter"); }
    bool IsActiveThreadTerminating() { return g_termAt && ++g_calls >= g_termAt; }
    void Main();
};
void CBalverineFightingGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing attacker = Game->GetNearestWithScriptName(Game->GetHero(), "MinionAttackingGuard");
    if (!ExplainedAboutSummoning) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(Me, attacker);
        Game->GiveThingBestEnemyTarget(attacker, Me);
        while (!ParentClass->QuestStartScreened) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->DisplayQuestInfo(true);
        HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_GUARD", 1.0f);
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)g_pScriptDef->GuardTalkDistance)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_SO_POWERFUL", false, Me, CScriptThing());
        ExplainedAboutSummoning = true;
        while (reinterpret_cast<const CScriptThingView&>(attacker).Thing().IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroMorality(g_pScriptDef->GuardRescueMorality);
        conversation = Game->AddNewConversation(Me, false, false);
        Game->AddLineToConversation(conversation, "TEXT_QST_045_GUARD_THANKS", false, Me, CScriptThing());
        Game->EntityFollowThing(Me, Game->GetHero(), 3.0f, true);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    }
    ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CQ_MinionClifftopChaseScript, CScriptThing>(CCharString("ParentClass.") + CCharString("GuardChatter"), 0, ParentClass, &CQ_MinionClifftopChaseScript::GuardChatter, Me), CCharString(""));
    while (!ParentClass->MissionFailed && !ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}

static std::string run(bool explained, int termAt)
{
    CGameScriptInterface game; CQ_MinionClifftopChaseScript parent = { false, false, false, "", -1 };
    g_parent = &parent; g_log = ""; g_calls = 0; g_termAt = termAt; g_frames = 0;
    g_screenAt = 3; g_dist = 5; g_missionAt = 0; g_alive[3] = 2;
    CBalverineFightingGuard g; g.Game = &game; g.Me = CScriptThing(2); g.ParentClass = &parent; g.PMasterData = 0;
    g.ExplainedAboutSummoning = explained; g.HealthBarID = 0;
    g_missionAt = 12;
    g.Main();
    if (!termAt) {
        CHECK(g.ExplainedAboutSummoning);
        CHECK(parent.Spawned == "ParentClass.GuardChatter" && parent.SpawnedPar == 2);
        if (!explained) CHECK(g.HealthBarID == 77);
    }
    return g_log;
}
int main()
{
    std::string full = run(false, 0);
    CHECK(full == "filter;|;nearest;enemy2>3;enemy3>2;|;|;|;questinfo;bar;|;conv;TEXT_QST_045_GUARD_SO_POWERFUL;alive?;|;alive?;|;alive?;morality;conv;TEXT_QST_045_GUARD_THANKS;follow;region;spawn;|;|;|;|;|;remove;");
    std::string skip = run(true, 0);
    CHECK(skip == "filter;|;nearest;spawn;|;|;|;|;|;|;|;|;|;|;|;remove;");
    // termination at every check point: the log must be a strict prefix of the full run and never reach remove
    for (int n = 1; n < 40; ++n) {
        std::string s = run(false, n);
        if (g_calls < n) { CHECK(s == full); break; }
        CHECK(full.compare(0, s.size(), s) == 0);
        CHECK(s.find("remove") == std::string::npos);
    }
    if (g_fail) { printf("FAILED\n%s\n%s\n", full.c_str(), skip.c_str()); return 1; }
    printf("00da6d50_TEST PASS\n");
    return 0;
}