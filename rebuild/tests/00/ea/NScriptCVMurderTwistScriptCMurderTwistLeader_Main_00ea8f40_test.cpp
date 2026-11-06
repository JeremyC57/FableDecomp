// Behaviour test for NScript::CV_MurderTwistScript::CMurderTwistLeader::Main (0x00EA8F40): while awaiting help, each talk tells the
// story and asks the yes/no question (a "no" keeps him waiting, a "yes" sets state 2 / event 2); in states 2..4 a talk with no new
// item found repeats the hint, otherwise he pays (200, then 500, then nothing), advances the state (3/3, 4/5, then 5) and gloats;
// afterwards he idles until the thread terminates. A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World { int Frame, TerminateAt; int Talk[16]; int Answers[4]; int NAnswer; int Gold; };
struct Leader {
    World* W; int State, Event, ItemsFound;
    bool T() { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    void NSF() { ++W->Frame; }
    bool Talked() { for (int i = 0; i < 16; ++i) if (W->Talk[i] == W->Frame) return true; return false; }
    void Main()
    {
        NSF();
        if (T()) return;
        while (State == 1) {
            NSF(); if (T()) return;
            if (Talked()) {
                if (T()) return;
                Log("story");
                long answer = W->Answers[W->NAnswer++];
                if (T()) return;
                if (answer == 1) { Log("thanks"); State = 2; Event = 2; } else Log("shame");
            }
        }
        if (T()) return;
        while (State == 2) {
            NSF(); if (T()) return;
            if (Talked()) {
                if (T()) return;
                if (ItemsFound == 0) Log("hint1"); else { Log("gem"); W->Gold += 200; State = 3; Event = 3; }
            }
        }
        if (T()) return;
        while (State == 3) {
            NSF(); if (T()) return;
            if (Talked()) {
                if (T()) return;
                if (ItemsFound == 1) Log("hint2"); else { Log("inheritance"); W->Gold += 500; State = 4; Event = 5; }
            }
        }
        if (T()) return;
        while (State == 4) {
            NSF(); if (T()) return;
            if (Talked()) {
                if (T()) return;
                if (ItemsFound == 2) Log("hint3"); else { Log("zilch"); State = 5; }
            }
        }
        for (;;) { if (T()) return; NSF(); }
    }
};
int main()
{
    World w = { 0, 40, { 2, 3, 5, 6, 8, 10 }, { 0, 1 }, 0, 0 };
    Leader l = { &w, 1, 0, 0 };
    g_log[0] = 0;
    l.Main();
    bool ok = !std::strcmp(g_log, "story;shame;story;thanks;hint1;hint1;hint1;hint1;") && l.State == 2 && l.Event == 2 && w.Gold == 0 && w.Frame == 40;
    World w2 = { 0, 30, { 2, 3, 4, 5, 6, 7 }, { 1 }, 0, 0 };
    Leader l2 = { &w2, 1, 0, 3 };
    g_log[0] = 0;
    l2.Main();
    ok = ok && !std::strcmp(g_log, "story;thanks;gem;inheritance;zilch;") && l2.State == 5 && l2.Event == 5 && w2.Gold == 700;
    if (!ok) std::printf("%s\n", g_log);
    std::printf(ok ? "00ea8f40_TEST PASS\n" : "00ea8f40_TEST FAIL\n");
    return ok ? 0 : 1;
}