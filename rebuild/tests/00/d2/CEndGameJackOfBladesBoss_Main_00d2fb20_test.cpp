// Behaviour test for CEndGameJackOfBladesBoss::Main (0x00D2FB20): filter registration, marker, the readiness wait and
// the targeting hand-over, with the IsDone early exits.
#include <cstdio>
#include <cstring>
struct CCharString { char text[32]; CCharString(const char* t, long = -1) { std::strcpy(text, t); } ~CCharString() {} };
struct CScriptThing { int id; };
struct CQ_EndGameBossBattleScript { bool JackReadyToFight; };
static int g_frames = 0, g_markers = 0, g_targets = 0, g_filters = 0; static char g_marker[32]; static int g_targetId = 0;
static CScriptThing g_hero = { 42 };
static CQ_EndGameBossBattleScript* g_parent = 0; static int g_readyAfter = 0;
struct CGameScriptInterface {
    virtual void NewScriptFrame() const { ++g_frames; if (g_frames == g_readyAfter) g_parent->JackReadyToFight = true; }
    virtual const CScriptThing& GetHero() const { return g_hero; }
    virtual void MiniMapAddMarker(const CScriptThing&, const CCharString& g) const { ++g_markers; std::strcpy(g_marker, g.text); }
    virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) const { ++g_targets; g_targetId = t.id; }
};
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} };
class CEndGameJackOfBladesBoss {
public:
    CGameScriptInterface* Game; CScriptThing Me; CQ_EndGameBossBattleScript* ParentClass; int doneAfter;
    void RegisterFilter(CIsThingAlive& f) { if (f.m_thing.id == Me.id) ++g_filters; }
    bool IsDone() { return g_frames >= doneAfter; }
    void Main();
};
void CEndGameJackOfBladesBoss::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    while (ParentClass->JackReadyToFight != true) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}
int main()
{
    CGameScriptInterface game; CQ_EndGameBossBattleScript parent = { false }; g_parent = &parent;
    CEndGameJackOfBladesBoss s; s.Game = &game; s.Me.id = 7; s.ParentClass = &parent;
    int bad = 0;
    s.doneAfter = 1; s.Main();   // done right after the first frame
    if (g_filters != 1 || g_markers || g_frames != 1) bad = 1;
    g_frames = 0; s.doneAfter = 10; g_readyAfter = 4; s.Main();
    if (g_markers != 1 || std::strcmp(g_marker, "HUD_ORB_QUEST_CORE") || g_targets != 1 || g_targetId != 42 || g_frames != 10) bad = 2;
    g_frames = 0; parent.JackReadyToFight = false; g_readyAfter = 100; s.doneAfter = 3; s.Main();
    if (g_targets != 1 || g_frames != 3) bad = 3;   // never ready: leaves when done
    if (bad) { std::printf("00d2fb20_TEST FAIL\n"); return 1; }
    std::printf("00d2fb20_TEST PASS\n");
    return 0;
}