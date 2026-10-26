// 00e06050 behaviour test: CQ_TraderEscortScript::WatchForSurprisingBalverines (match.cpp body verbatim, host stand-ins)
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string g_log;
static int g_calls = 0, g_termAt = 0, g_frames = 0, g_rand = 0;
static void L(const char* s) { g_log += s; g_log += ";"; }
static int host_rand() { return g_rand++; }
#define rand host_rand
struct C3DVector { float X, Y, Z; };
struct CCharString { std::string S; CCharString(const char* t, long n = -1) : S(t) { (void)n; } };
enum ECutsceneBehaviour { CUTSCENE_BEHAVIOUR_NONE };
enum ETraderComment { TRADER_COMMENT_NONE };
static int g_hp[16];
static int g_things = 0;
struct CScriptThing {
    int Id; C3DVector Pos;
    CScriptThing() : Id(0) { Pos.X = Pos.Y = Pos.Z = 0; ++g_things; }
    CScriptThing(const CScriptThing& o) : Id(o.Id), Pos(o.Pos) { ++g_things; }
    explicit CScriptThing(int id) : Id(id) { Pos.X = (float)id; Pos.Y = Pos.Z = 0; ++g_things; }
    virtual ~CScriptThing() { --g_things; }
    const C3DVector& GetPos() const { return Pos; }
    virtual bool IsAlive() const { L("alive?"); return g_hp[Id] > 0; }
};
typedef std::vector<CScriptThing> CScriptThingVector;
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};
static int g_onScreenMask = 0, g_cur = 0;
struct CGameScriptInterface {
    CScriptThing Hero;
    CGameScriptInterface() : Hero(1) {}
    void NewScriptFrame() const { ++g_frames; L("|"); if (g_cur && g_hp[g_cur] > 0) --g_hp[g_cur]; }
    CScriptThing& GetHero() { return Hero; }
    long GetAllThingsWithScriptName(const CCharString& n, CScriptThingVector& v) const { CHECK(n.S == "M_BalverineSurprise"); for (int k = 0; k < 3; ++k) v.push_back(CScriptThing(10 + k)); L("markers"); return 3; }
    CScriptThing GetNearestWithScriptName(const CScriptThing& t, const CCharString& n) const { CHECK(n.S == "DarkwoodTrader"); (void)t; return CScriptThing(5); }
    void SetTimer(long timer, long time) { CHECK(timer == 9 && time == 0); L("timer"); }
    CScriptThing CreateCreature(const CCharString& d, const C3DVector& p, const CCharString& s, bool a) { CHECK(d.S == "CREATURE_BALVERINE_EASY" && s.S == "SurpriseBalverine" && a); char b[16]; sprintf(b, "create%d", (int)p.X); L(b); g_hp[(int)p.X - 8] = 2; return CScriptThing((int)p.X - 8); }
    void Pause(float s) const { CHECK(s == 2.0f); L("pause"); }
    bool IsCameraPosOnScreen(const C3DVector& p) const { return ((g_onScreenMask >> ((int)p.X - 10)) & 1) != 0; }
    void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& g) { char b[16]; sprintf(b, "enemy%d>%d", t.Id, g.Id); L(b); g_cur = t.Id; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing& t, const CScriptThing& g, bool a) const { CHECK(g.Id == 1 && !a); (void)t; L("face"); }
    void EntitySetCutsceneBehaviour(const CScriptThing& t, ECutsceneBehaviour b) const { CHECK((int)b == 1); (void)t; L("cutscene"); }
};
static CGameScriptInterface* g_pGameScriptInterface;
struct CTimer { long Index; void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); } };
namespace NScript {
struct CScriptBase { bool IsActiveThreadTerminating() { bool t = g_termAt && ++g_calls >= g_termAt; return t; } };
struct CQ_TraderEscortScript : CScriptBase {
    CGameScriptInterface* GSI; void* PMasterData; CTimer CommentTimer; long BalverinesToSurpriseHeroNeeded;
    void WatchForSurprisingBalverines();
    bool MakeTraderComment(const CCharString& c, const CScriptThing& trader, ETraderComment type) { CHECK(c.S == "BALVERINE_SURPRISE" && trader.Id == 5 && (int)type == 0); L("comment"); return true; }
};
void CQ_TraderEscortScript::WatchForSurprisingBalverines()
{
    CScriptThingVector markers;
    for (;;) {
        if (IsActiveThreadTerminating())
            break;
        if (BalverinesToSurpriseHeroNeeded > 0) {
            if (IsActiveThreadTerminating())
                return;
            GSI->Pause(2.0f);
            markers.clear();
            GSI->GetAllThingsWithScriptName("M_BalverineSurprise", markers);
            for (unsigned int i = 0; i < markers.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (BalverinesToSurpriseHeroNeeded > 0 && !GSI->IsCameraPosOnScreen(markers[i].GetPos())) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    CScriptThing balverine = GSI->CreateCreature("CREATURE_BALVERINE_EASY", markers[i].GetPos(), "SurpriseBalverine", true);
                    GSI->EntitySetCutsceneBehaviour(balverine, (ECutsceneBehaviour)1);
                    GSI->EntitySetFacingAngleTowardsThing(balverine, GSI->GetHero(), false);
                    CommentTimer.SetTime(0);
                    MakeTraderComment("BALVERINE_SURPRISE", GSI->GetNearestWithScriptName(balverine, "DarkwoodTrader"), (ETraderComment)0);
                    if (rand() % 5 == 0) {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        GSI->GiveThingBestEnemyTarget(balverine, GSI->GetNearestWithScriptName(balverine, "DarkwoodTrader"));
                    } else {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        GSI->GiveThingBestEnemyTarget(balverine, GSI->GetHero());
                    }
                    while (reinterpret_cast<const CScriptThingView&>(balverine).Thing().IsAlive()) {
                        GSI->NewScriptFrame();
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                    }
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    --BalverinesToSurpriseHeroNeeded;
                    break;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        GSI->NewScriptFrame();
    }
}
}

static std::string run(int termAt, int needed, int mask)
{
    CGameScriptInterface gsi; g_pGameScriptInterface = &gsi;
    g_log = ""; g_calls = 0; g_termAt = termAt; g_frames = 0; g_rand = 0; g_onScreenMask = mask; g_cur = 0;
    NScript::CQ_TraderEscortScript s; s.GSI = &gsi; s.PMasterData = 0; s.CommentTimer.Index = 9; s.BalverinesToSurpriseHeroNeeded = needed;
    int before = g_things;
    s.WatchForSurprisingBalverines();
    CHECK(g_things == before);
    return g_log;
}
int main()
{
    // two balverines needed, marker 10 on screen: each pass spawns at marker 11 (rand 0 -> trader target, rand 1 -> hero target)
    std::string full = run(40, 2, 1);
    CHECK(full.find("pause;markers;create11;cutscene;face;timer;comment;enemy3>5;alive?;|;alive?;|;alive?;") == 0);
    CHECK(full.find("create11;cutscene;face;timer;comment;enemy3>1;alive?;|;alive?;|;alive?;", 10) != std::string::npos);
    // nothing needed: frames only, until termination
    CHECK(run(4, 0, 0) == "|;|;|;");
    CHECK(run(1, 2, 0) == "");
    // every marker on screen: no spawn, one pause per pass
    std::string none = run(9, 1, 7);
    CHECK(none.find("create") == std::string::npos && none.find("pause;markers;|;pause;markers;") == 0);
    for (int n = 1; n < 40; ++n) {
        std::string s = run(n, 2, 1);
        CHECK(full.compare(0, s.size(), s) == 0);
    }
    if (g_fail) { printf("FAILED %s\n", full.c_str()); return 1; }
    printf("00e06050_TEST PASS\n");
    return 0;
}