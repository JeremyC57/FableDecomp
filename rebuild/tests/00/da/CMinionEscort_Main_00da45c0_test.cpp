// behaviour copy of CMinionEscort::Main
#include <cstdio>
#include <cstring>
static int g_seq[32]; static int g_n = 0; static int g_frames = 0, g_doneAfter = 3, g_filters = 0;
struct CScriptThing { int id; };
struct CCharString { const char* p; };
struct CGameScriptInterface {
    CScriptThing hero; const char* attached; const CScriptThing* faced; const CScriptThing* target;
    void NewScriptFrame() { ++g_frames; g_seq[g_n++] = 1; }
    const CScriptThing& GetHero() const { return hero; }
    CCharString GetActiveQuestName() { CCharString s = { "Q_MINION" }; return s; }
    void EntityAttachToScript(const CScriptThing&, const CCharString& s) { attached = s.p; g_seq[g_n++] = 2; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing& t, bool) { faced = &t; g_seq[g_n++] = 3; }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& t) { target = &t; g_seq[g_n++] = 4; }
};
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} };
struct CMinionEscort {
    CGameScriptInterface* Game; CScriptThing Me;
    void RegisterFilter(CIsThingAlive& f) { if (f.m_thing.id == Me.id) ++g_filters; }
    bool IsDone() { return g_frames >= g_doneAfter; }
    void Main()
    {
        {
            CIsThingAlive filter(Me);
            RegisterFilter(filter);
        }
        Game->NewScriptFrame();
        if (!IsDone()) {
            Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            do {
                Game->NewScriptFrame();
            } while (!IsDone());
        }
    }
};
int main()
{
    CGameScriptInterface g; g.hero.id = 1; g.attached = 0; g.faced = 0; g.target = 0;
    CMinionEscort e; e.Game = &g; e.Me.id = 7; e.Main();
    if (g_filters != 1 || g_frames != 3 || std::strcmp(g.attached, "Q_MINION") != 0 || g.faced != &g.hero || g.target != &g.hero
        || g_n != 6 || g_seq[0] != 1 || g_seq[1] != 2 || g_seq[2] != 3 || g_seq[3] != 4 || g_seq[5] != 1)
    { std::printf("00da45c0_TEST FAIL\n"); return 1; }
    g_frames = 0; g_doneAfter = 1; g_n = 0; e.Main();
    if (g_n != 1 || g_frames != 1) { std::printf("00da45c0_TEST FAIL\n"); return 1; }
    std::printf("00da45c0_TEST PASS\n");
    return 0;
}