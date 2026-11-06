// Behaviour test for NScript::CPersonalScript_EmptyScript::Main (0x00EF0090): clear ReachedGoalInStealthMission, thirty bindings,
// post-add, spawn CheckForHeroActions, activate TestQuestCard1, activate the guild teleporter when it exists, then each frame report
// the hero's death and the hero's sleep (daytime / night time) until the thread terminates. A state model, run for three scenarios.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Script {
    bool ReachedGoalInStealthMission, TeleporterAlive;
    int Frames, TerminateAt, DieAt, SleepAt;
    bool SleptDay;
    void NewScriptFrame() { ++Frames; }
    bool IsActiveThreadTerminating() { return Frames >= TerminateAt; }
    bool HeroKilled() { return Frames == DieAt; }
    bool HeroSlept(bool& day) { day = SleptDay; return Frames == SleepAt; }
    void Main()
    {
        ReachedGoalInStealthMission = false;
        Log("bind30"); Log("post"); Log("spawn"); Log("quest");
        if (TeleporterAlive) Log("teleporter");
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (HeroKilled()) {
                if (IsActiveThreadTerminating()) return;
                Log("died"); NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            bool day;
            if (HeroSlept(day)) {
                if (IsActiveThreadTerminating()) return;
                if (day) { if (IsActiveThreadTerminating()) return; Log("day"); NewScriptFrame(); }
                else { if (IsActiveThreadTerminating()) return; Log("night"); NewScriptFrame(); }
                if (IsActiveThreadTerminating()) return;
            }
            NewScriptFrame();
        }
    }
};
int main()
{
    Script a = { true, true, 0, 6, 1, 3, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "bind30;post;spawn;quest;teleporter;died;day;") && !a.ReachedGoalInStealthMission && a.Frames == 6;
    g_log[0] = 0;
    Script b = { true, false, 0, 4, -1, 2, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "bind30;post;spawn;quest;night;") && b.Frames == 4;
    g_log[0] = 0;
    Script c = { true, true, 0, 0, 0, 0, true };
    c.Main();
    ok = ok && !std::strcmp(g_log, "bind30;post;spawn;quest;teleporter;") && c.Frames == 0;
    std::printf(ok ? "00ef0090_TEST PASS\n" : "00ef0090_TEST FAIL\n");
    return ok ? 0 : 1;
}