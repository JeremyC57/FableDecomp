// Behaviour test for CHTG_Prisoner::Main (0x00D71B30): the escape loop of quest state 5 -- the leader calls the guards once the
// prisoner is over 12 units away, the prisoner walks to the exit when idle, dies at the exit (mission failed), and returns to the
// party when the hero gets within 3 units (taking the "returning" line only if the guards had been called).
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }

struct Frame { int state; float leaderDist; float exitDist; float heroDist; int busy; };
struct World { int frame; int frames; const Frame* f; };
static World w;

struct Parent {
    long HTG_State; bool PrisonerEscaping; bool BanditsFlee; bool MissionFailed; const char* ReasonForFailing;
};
struct CHTG_Prisoner {
    Parent* ParentClass;
    bool ReturningToParty;
    bool GuardsCalledAfterMe;
    void NewScriptFrame() { ++w.frame; if (w.frame < w.frames) ParentClass->HTG_State = w.f[w.frame].state; }
    bool IsActiveThreadTerminating() const { return w.frame >= w.frames - 1; }
    const Frame& F() const { return w.f[w.frame]; }
    void Main();
};
void CHTG_Prisoner::Main()
{
    while (ParentClass->HTG_State != 7) {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->HTG_State == 5) {
            GuardsCalledAfterMe = false;
            Log("clear");
            while (ParentClass->HTG_State == 5) {
                NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (!GuardsCalledAfterMe && !ReturningToParty && F().leaderDist > 12.0f) {
                    Log("ESCAPING");
                    GuardsCalledAfterMe = true;
                } else if (!F().busy) {
                    Log("move");
                    ParentClass->PrisonerEscaping = true;
                    ReturningToParty = false;
                } else if (F().exitDist < 1.5f) {
                    Log("die");
                    ParentClass->BanditsFlee = true;
                    ParentClass->ReasonForFailing = "ESCAPED";
                    ParentClass->MissionFailed = true;
                } else {
                    bool closeToHero = F().heroDist < 3.0f;
                    if (closeToHero && !ReturningToParty) {
                        Log("follow");
                        if (GuardsCalledAfterMe) {
                            Log("RETURNING");
                            GuardsCalledAfterMe = false;
                        }
                        ReturningToParty = true;
                        ParentClass->PrisonerEscaping = false;
                    } else if (ReturningToParty && F().heroDist > 4.0f) {
                        Log("clear");
                        ReturningToParty = false;
                    }
                }
            }
        }
    }
}
static bool Run(const Frame* f, int n, const char* expect, bool failed)
{
    Parent p; p.HTG_State = f[0].state; p.PrisonerEscaping = false; p.BanditsFlee = false; p.MissionFailed = false; p.ReasonForFailing = "";
    CHTG_Prisoner s; s.ParentClass = &p; s.ReturningToParty = false; s.GuardsCalledAfterMe = false;
    w.frame = 0; w.frames = n; w.f = f;
    g_log[0] = 0;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return p.MissionFailed == failed;
}
int main()
{
    static const Frame escape[] = {
        { 0, 0, 0, 0, 0 }, { 5, 5, 20, 20, 1 }, { 5, 5, 20, 20, 0 }, { 5, 13, 10, 20, 1 }, { 5, 14, 1, 20, 1 }, { 7, 0, 20, 20, 1 }, { 7, 0, 0, 0, 0 } };
    static const Frame caught[] = {
        { 0, 0, 0, 0, 0 }, { 5, 5, 20, 20, 1 }, { 5, 13, 10, 20, 1 }, { 5, 13, 10, 2, 1 }, { 5, 13, 10, 5, 1 }, { 7, 0, 20, 20, 1 }, { 7, 0, 0, 0, 0 } };
    if (!Run(escape, 7, "clear;move;ESCAPING;die;", true)
        || !Run(caught, 7, "clear;ESCAPING;follow;RETURNING;clear;", false)) {
        std::printf("00d71b30_TEST FAIL\n");
        return 1;
    }
    std::printf("00d71b30_TEST PASS\n");
    return 0;
}