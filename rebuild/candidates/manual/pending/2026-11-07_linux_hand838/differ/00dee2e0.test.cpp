// Behaviour test for NScript::CQ_SecretPassageScript::CMotherTorture::Main (0x00DEE2E0): unless already found, the mother waits for the
// hero to pull the cell lever, then opens the door and is found; then she follows the hero and each frame answers a talk (LETS_GO), a
// hero hit (ON_HIT) or, when her speech timer has run out, comments (HIT_BY_ENEMY, FIGHTING_COMPLIMENT on a combo > 3 with rand % 100
// == 0, USE_FLOURISH with rand % 200 == 0, NOT_FAR_NOW with rand % 400 == 0) and rearms the timer for 15. A frame-stepped model with a
// scripted rand and a timer that counts one per frame.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Mother {
    int Frame, TerminateAt, LeverAt, TalkAt, HeroHitAt, EnemyHitAt, Combo, Timer, Roll;
    bool Found, Flourish;
    const int* Rolls;
    void NSF() { ++Frame; if (Timer > 0) --Timer; }
    bool T() const { return Frame >= TerminateAt; }
    int Rand() { return Rolls[Roll++]; }
    void Main()
    {
        NSF(); if (T()) return;
        if (!Found) {
            while (Frame < LeverAt) { NSF(); if (T()) return; }
            Log("door");
            Found = true;
        }
        Log("follow");
        for (;; NSF()) {
            if (T()) return;
            if (Frame == TalkAt) Log("LETS_GO");
            if (Frame == HeroHitAt) Log("ON_HIT");
            else if (Timer == 0) {
                if (Frame == EnemyHitAt) { Log("HIT_BY_ENEMY"); Timer = 15; }
                else if (Combo > 3 && Rand() % 100 == 0) { Log("COMPLIMENT"); Timer = 15; }
                else if (Flourish && Rand() % 200 == 0) { Log("FLOURISH"); Timer = 15; }
                else if (Rand() % 400 == 0) { Log("NOT_FAR_NOW"); Timer = 15; }
            }
        }
    }
};
int main()
{
    // timer starts at 5 (runs out at frame 5); frame 3 lever; 4 talk; 5 hero hit; 6 enemy hit (timer 15 -> 0 at 21); 21: combo roll 0 -> compliment (timer 0 at 36);
    // 36: combo roll 1, flourish roll 200 -> flourish (timer 0 at 51); 51..: rolls 7, 3, 1 -> no line until termination at 53
    static const int rolls[] = { 0, 1, 200, 7, 3, 1, 5, 5, 5 };
    Mother a = { 0, 53, 3, 4, 5, 6, 4, 5, 0, false, true, rolls };
    a.Main();
    bool ok = !std::strcmp(g_log, "door;follow;LETS_GO;ON_HIT;HIT_BY_ENEMY;COMPLIMENT;FLOURISH;") && a.Found && a.Frame == 53;
    g_log[0] = 0;
    static const int rolls2[] = { 400, 1, 1, 1 };
    Mother b = { 0, 4, 1, 50, 50, 50, 0, 0, 0, true, false, rolls2 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "follow;NOT_FAR_NOW;") && b.Frame == 4;
    g_log[0] = 0;
    Mother c = { 0, 3, 10, 50, 50, 50, 0, 0, 0, false, false, rolls2 };
    c.Main();
    ok = ok && g_log[0] == 0 && !c.Found && c.Frame == 3;
    std::printf(ok ? "00dee2e0_TEST PASS\n" : "00dee2e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
