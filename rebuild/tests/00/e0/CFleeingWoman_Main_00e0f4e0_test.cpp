// Behaviour test for 0x00E0F4E0: the call sequence of CFleeingWoman::Main (wait for the hero, talk once, run to the escape point, fade out).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: int id; };
struct CDef { int trigger; int talk; float fade; };
static CDef g_def;
class CGame { public:
    const char* dist;   // script of answers for IsDistanceBetweenThingsUnder: 'y' / 'n'
    int distIdx;
    int busy;           // IsPerformingScriptTask answers true this many times first
    CScriptThing hero, escape;
    void NewScriptFrame() { Log("frame"); }
    const CScriptThing& GetHero() { return hero; }
    bool StartScriptingEntity(int priority) { Log(priority == 4 ? "start4" : "startBad"); return true; }
    CScriptThing GetThingWithScriptName(const char* n) { Log(std::strcmp(n, "FleeingWomanEscapePos") == 0 ? "escape" : "escapeBad"); return escape; }
    bool IsDistanceUnder(const CScriptThing& b, float d)
    {
        char buf[32];
        std::sprintf(buf, "dist%d@%g", b.id, d);
        Log(buf);
        return dist[distIdx++] == 'y';
    }
    int AddNewConversation() { Log("conv"); return 42; }
    void AddPerson(int c, const CScriptThing& t) { Log(c == 42 && t.id == 1 ? "person" : "personBad"); }
    void AddLine(int c, const char* text) { Log(c == 42 && std::strcmp(text, "TEXT_QST_072_WOMAN_FLEES_10") == 0 ? "line" : "lineBad"); }
    bool IsPerforming() { Log("busy?"); if (busy > 0) { --busy; return true; } return false; }
    void MoveTo(const CScriptThing& t) { Log(t.id == 9 ? "move" : "moveBad"); }
    void Fade(float s) { Log(s == 3.5f ? "fade" : "fadeBad"); }
};
struct CScript {
    CGame* Game;
    bool Talked;
    int terminateAt, calls;
    bool IsDone() { ++calls; return calls == terminateAt; }
    void Main()
    {
        Game->NewScriptFrame();
        if (IsDone()) return;
        while (!Game->StartScriptingEntity(4)) { Game->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        while (!Game->IsDistanceUnder(Game->GetHero(), (float)g_def.trigger)) { Game->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        CScriptThing target = Game->GetThingWithScriptName("FleeingWomanEscapePos");
        if (!IsDone()) {
            for (;;) {
                if (!Talked && Game->IsDistanceUnder(Game->GetHero(), (float)g_def.talk)) {
                    if (IsDone()) return;
                    Talked = true;
                    int c = Game->AddNewConversation();
                    Game->AddPerson(c, Game->GetHero());
                    Game->AddLine(c, "TEXT_QST_072_WOMAN_FLEES_10");
                }
                if (!Game->IsPerforming()) {
                    if (IsDone()) return;
                    Game->MoveTo(target);
                }
                if (Game->IsDistanceUnder(target, 2.0f)) break;
                Game->NewScriptFrame();
                if (IsDone()) return;
            }
            if (IsDone()) return;
            Game->Fade(g_def.fade);
        }
    }
};
static int Run(const char* dist, int busy, int terminateAt, const char* expect)
{
    CGame g; g.dist = dist; g.distIdx = 0; g.busy = busy; g.hero.id = 1; g.escape.id = 9;
    CScript s; s.Game = &g; s.Talked = false; s.terminateAt = terminateAt; s.calls = 0;
    g_log[0] = 0;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\nexp %s\n", g_log, expect); return 1; }
    return 0;
}
int main()
{
    int bad = 0;
    g_def.trigger = 20; g_def.talk = 8; g_def.fade = 3.5f;
    bad += Run("", 0, 1, "frame;");
    bad += Run("nyyny", 1, 0, "frame;start4;dist1@20;frame;dist1@20;escape;dist1@8;conv;person;line;busy?;dist9@2;frame;busy?;move;dist9@2;fade;");
    bad += Run("ynnny", 0, 0, "frame;start4;dist1@20;escape;dist1@8;busy?;move;dist9@2;frame;dist1@8;busy?;move;dist9@2;fade;");
    bad += Run("y", 0, 4, "frame;start4;dist1@20;escape;");
    if (bad) { std::printf("00e0f4e0_TEST FAIL\n"); return 1; }
    std::printf("00e0f4e0_TEST PASS\n");
    return 0;
}