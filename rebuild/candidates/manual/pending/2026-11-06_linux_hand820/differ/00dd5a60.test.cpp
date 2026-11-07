// Behaviour test for NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main (0x00DD5A60): a comment timer starts at
// rand() % 90; until released the prisoner makes a RELEASE (hero escaped his cell) or IDLE comment whenever the timer is 0 and the
// comment is made (timer reset to 30), and when the hero uses the cell door he waits for the running conversation, thanks the hero
// and is released; then he follows the hero (persistent, region-following, prisoner faction) making COURTYARD comments and
// complaining when hit until the courtyard region is loaded; then region-following stops and he makes GUARDS comments forever.
// A log model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct World { int Frame, DoorUsedAt, CourtyardAt, HitAt, TerminateAt, Timer; bool HeroEscapedCell; };
struct Prisoner {
    World* W; bool Released;
    bool T() { return W->TerminateAt > 0 && W->Frame >= W->TerminateAt; }
    void NewScriptFrame() { ++W->Frame; if (W->Timer > 0) --W->Timer; Log("frame"); }
    bool Comment(const char* s) { Log(s); return true; }
    void Main(int rnd)
    {
        W->Timer = rnd % 90;
        if (!Released) {
            if (T()) return;
            while (!Released) {
                NewScriptFrame();
                if (T()) return;
                if (W->HeroEscapedCell && W->Timer == 0 && Comment("RELEASE")) { if (T()) return; W->Timer = 30; }
                if (!W->HeroEscapedCell && W->Timer == 0 && Comment("IDLE")) { if (T()) return; W->Timer = 30; }
                if (W->Frame == W->DoorUsedAt) { if (T()) return; Log("TEXT_QST_055_PRISON_PRISONER_FREE"); Released = true; }
            }
            if (T()) return;
        }
        Log("follow");
        while (W->Frame < W->CourtyardAt) {
            NewScriptFrame();
            if (T()) return;
            if (W->Timer == 0 && Comment("COURTYARD")) { if (T()) return; W->Timer = 30; }
            if (W->Frame == W->HitAt) { if (T()) return; Log("ON_HIT"); }
        }
        if (T()) return;
        Log("unfollow");
        for (;;) {
            if (T()) return;
            if (W->Timer == 0 && Comment("GUARDS")) { if (T()) return; W->Timer = 30; }
            if (W->Frame == W->HitAt) { if (T()) return; Log("ON_HIT"); }
            NewScriptFrame();
        }
    }
};
int main()
{
    World w = { 0, 2, 4, 3, 6, 0, false };
    Prisoner p = { &w, false };
    g_log[0] = 0; p.Main(95);                     // timer 5: no comment before the courtyard
    bool ok = !std::strcmp(g_log, "frame;frame;TEXT_QST_055_PRISON_PRISONER_FREE;follow;frame;ON_HIT;frame;unfollow;frame;GUARDS;frame;");
    World w2 = { 0, 0, 0, 0, 2, 0, true };
    Prisoner p2 = { &w2, false };
    g_log[0] = 0; p2.Main(0);
    ok = ok && !std::strcmp(g_log, "frame;RELEASE;frame;");
    if (!ok) std::printf("%s\n", g_log);
    std::printf(ok ? "00dd5a60_TEST PASS\n" : "00dd5a60_TEST FAIL\n");
    return ok ? 0 : 1;
}
