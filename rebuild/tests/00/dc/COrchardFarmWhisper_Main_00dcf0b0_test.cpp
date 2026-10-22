// Behaviour test for 0x00DCF0B0 COrchardFarmWhisper::Main: wait out the cutscene, set up the block tutorial and the
// flourish whisper, wait for the hero's flourish hit, whisper again, arm the health bar, then flag the mission won
// whenever Whisper's health is at or below 1 until the thread terminates.
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
static void Frame() { Log("frame"); }
struct Whisper {
    bool won;
    void Main()
    {
        Log("pred");
        while (Ans("cutscene")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("killable:0,0"); Log("combo:0"); Log("combat:BLOCK");
        Log("conv"); Log("person:hero"); Log("line:ATTACK_WITH_FLOURISH");
        while (!Ans("flourish")) { Frame(); if (Term()) return; }
        if (Term()) return;
        Log("conv"); Log("person:hero"); Log("line:HAS_BEEN_HIT");
        Log("combo:1"); Log("killable:0,1"); Log("damageable:1"); Log("questinfo:1"); Log("bar:HUD_WHISPER_ICON");
        Log("combat:OFARM");
        while (!Term()) {
            if (Ans("health<=1")) {
                if (Term()) return;
                won = true; Log("won");
            }
            Frame();
        }
    }
};
int main()
{
    int bad = 0; Whisper s;
    s.won = false; Reset("yy", 1); s.Main(); bad += Check("pred;cutscene;frame;");
    s.won = false; Reset("nnyny", 7); s.Main();
    bad += Check("pred;cutscene;killable:0,0;combo:0;combat:BLOCK;conv;person:hero;line:ATTACK_WITH_FLOURISH;flourish;frame;flourish;conv;person:hero;line:HAS_BEEN_HIT;combo:1;killable:0,1;damageable:1;questinfo:1;bar:HUD_WHISPER_ICON;combat:OFARM;health<=1;frame;health<=1;won;frame;");
    if (!s.won) bad++;
    if (bad) { std::printf("00dcf0b0_TEST FAIL\n"); return 1; }
    std::printf("00dcf0b0_TEST PASS\n"); return 0;
}