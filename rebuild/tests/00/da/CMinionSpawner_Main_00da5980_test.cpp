#include <cstdio>
#include <cstring>
#include <cstdlib>
// Behaviour test for CMinionSpawner::Main (0x00DA5980): trigger distance from the data string (10 when 0), wait for the hero,
// then summon effect, pause, war-dog minion facing the hero, and remove the spawner.
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CScriptThing { const char* Data; float Pos; const char* GetDataString() const { return Data; } float GetPos() const { return Pos; } };
struct CGame {
    int Frames, TerminateAt; float HeroPos, LastRange;
    CScriptThing Hero;
    void NewScriptFrame() { Frames++; HeroPos -= 1.0f; Log("F"); }
    const CScriptThing& GetHero() { Hero.Pos = HeroPos; return Hero; }
    CScriptThing CreateEffect(const char* e, float, const char*, float, bool, bool) { if (!std::strcmp(e, "SUMMON_CREATURE_SUMMON_EVIL")) Log("E"); CScriptThing t = { "", 0 }; return t; }
    void Pause(float s) { if (s == 0.2f) Log("P"); }
    CScriptThing CreateCreature(const char* d, float p, const char* n, bool b) { if (!std::strcmp(d, "CREATURE_MINION_WARDOG") && !std::strcmp(n, "MinionEscort") && b) Log("C"); CScriptThing t = { "dog", p }; return t; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing& t, const CScriptThing& h, bool) { if (!std::strcmp(t.Data, "dog") && &h == &Hero) Log("A"); }
    void RemoveThing(const CScriptThing&, bool a, bool b) { if (!a && b) Log("R"); }
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float d, CGame* g) { g->LastRange = d; return b.Pos - a.Pos < d; }
struct CMinionSpawner {
    CGame* Game; CScriptThing Me;
    bool IsDone() { return Game->Frames >= Game->TerminateAt; }
    void Main()
    {
        long distance = std::atol(Me.GetDataString());
        if (distance == 0) { if (IsDone()) return; distance = 10; }
        float range = (float)distance;
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), range, Game)) { Game->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        Game->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", Me.GetPos(), "", 0.0f, false, false);
        Game->Pause(0.2f);
        CScriptThing minion = Game->CreateCreature("CREATURE_MINION_WARDOG", Me.GetPos(), "MinionEscort", true);
        Game->EntitySetFacingAngleTowardsThing(minion, Game->GetHero(), false);
        Game->RemoveThing(Me, false, true);
    }
};
int main()
{
    CGame g = { 0, 100, 13.5f, 0.0f, { "", 0 } };
    CMinionSpawner m = { &g, { "", 0.0f } };
    m.Main();   // empty data -> range 10: hero at 13.5 needs 4 frames
    if (std::strcmp(g_log, "FFFFEPCAR") != 0 || g.LastRange != 10.0f) { std::printf("00da5980_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    CGame h = { 0, 2, 30.0f, 0.0f, { "", 0 } };
    CMinionSpawner n = { &h, { "5", 0.0f } };
    n.Main();   // range 5, thread ends after 2 frames
    if (std::strcmp(g_log, "FF") != 0 || h.LastRange != 5.0f) { std::printf("00da5980_TEST FAIL\n"); return 1; }
    std::printf("00da5980_TEST PASS\n");
    return 0;
}