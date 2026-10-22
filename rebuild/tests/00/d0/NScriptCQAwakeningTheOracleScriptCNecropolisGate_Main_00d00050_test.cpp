// Behaviour test for 0x00D00050 CNecropolisGate::Main: warn the hero near the gate (conversation + 15 s timer) until the
// opening cutscene has played, then (once) wait for the hero at CUTSCENE_TRIGGER, remove the guard, play the side or front
// intro (whichever entry is nearer), teleport the hero and idle; the timer is released on every exit.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_termAt, g_termCalls;
static bool Term() { ++g_termCalls; return g_termCalls == g_termAt; }
static const char* g_ans;
static int g_ansIdx;
static bool Ans(const char* what)
{
    Log(what);
    char c = g_ans[g_ansIdx];
    if (c) ++g_ansIdx;
    return c == 'y';
}
static void Reset(const char* answers, int termAt)
{
    g_log[0] = 0; g_ans = answers; g_ansIdx = 0; g_termAt = termAt; g_termCalls = 0;
}
static int Check(const char* expect)
{
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\nexp %s\n", g_log, expect); return 1; }
    return 0;
}
static long g_timerValue;
struct Timer {
    Timer() { Log("timer+"); }
    ~Timer() { Log("timer-"); }
    long GetTime() const { return g_timerValue; }
    void SetTime(long t) { g_timerValue = t; Log(t == 15 ? "settimer:15" : "settimer:?"); }
};
static float g_sideDist, g_frontDist;
struct Gate {
    bool playedOpeningCS, playedNecroCS;
    void Main()
    {
        Timer timer;
        Log("find:PretendGuard");
        while (!playedOpeningCS) {
            Log("frame");
            if (Term()) return;
            if (timer.GetTime() == 0 && Ans("near10")) {
                if (Term()) return;
                Log("conv"); Log("person:hero"); Log("line:TEXT_QST_B03_NECROPOLIS_BLOCKED");
                timer.SetTime(15);
            }
            if (Ans("openingcs")) playedOpeningCS = true;
        }
        if (Term()) return;
        if (!playedNecroCS) {
            if (Term()) return;
            Log("find:CUTSCENE_TRIGGER");
            while (!Ans("near20")) {
                Log("frame");
                if (Term()) return;
            }
            if (Term()) return;
            Log("remove:guard");
            bool sideEntry = g_sideDist < g_frontDist;
            if (sideEntry) {
                if (Term()) return;
                Log("cs:SIDE");
            }
            else {
                if (Term()) return;
                Log("cs:FRONT");
            }
            playedNecroCS = true;
            Log("teleport:NorthernWastes2HSP");
        }
        for (;;) {
            Log("idle");
            if (Term()) return;
        }
    }
};
int main()
{
    int bad = 0; Gate g;
    // warned once (timer 0 and near), the timer then blocks a second warning; the cutscene ends the first loop
    g.playedOpeningCS = false; g.playedNecroCS = true; g_timerValue = 0; Reset("yny", 5); g.Main();
    bad += Check("timer+;find:PretendGuard;frame;near10;conv;person:hero;line:TEXT_QST_B03_NECROPOLIS_BLOCKED;settimer:15;openingcs;frame;openingcs;idle;timer-;");
    // side entry nearer: trigger reached after one frame, side cutscene, teleport, idle until terminated
    g.playedOpeningCS = true; g.playedNecroCS = false; g_sideDist = 4.0f; g_frontDist = 9.0f; Reset("ny", 6); g.Main();
    bad += Check("timer+;find:PretendGuard;find:CUTSCENE_TRIGGER;near20;frame;near20;remove:guard;cs:SIDE;teleport:NorthernWastes2HSP;idle;timer-;");
    if (!g.playedNecroCS) bad++;
    // front entry nearer (ties go to the front)
    g.playedOpeningCS = true; g.playedNecroCS = false; g_sideDist = 9.0f; g_frontDist = 9.0f; Reset("y", 5); g.Main();
    bad += Check("timer+;find:PretendGuard;find:CUTSCENE_TRIGGER;near20;remove:guard;cs:FRONT;teleport:NorthernWastes2HSP;idle;timer-;");
    // terminated inside the first loop: the timer is still released
    g.playedOpeningCS = false; g.playedNecroCS = false; g_timerValue = 3; Reset("", 1); g.Main();
    bad += Check("timer+;find:PretendGuard;frame;timer-;");
    if (bad) { std::printf("00d00050_TEST FAIL\n"); return 1; }
    std::printf("00d00050_TEST PASS\n"); return 0;
}