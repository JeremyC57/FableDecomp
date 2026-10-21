// Behaviour test for CTCBerserk::CancelMove (0x0078C610): the cancel arm versus the timer rescale/clamp arms.
#include <cstdio>
struct State { bool Active; long St; };
struct Berserk {
    bool hasState; State* state; bool AbilityInitialised; long Timer; float Released; long AnimTimer; long MaxTimer;
    bool effectAlive; bool effectKilled; bool hasExit; int created; bool removed84;
    void CancelMove()
    {
        if (!hasState) return;
        if (!AbilityInitialised && state->St == 8) {
            state->Active = false; state->St = 0;
            if (effectAlive) effectKilled = true;
            if (hasExit) ++created;
            removed84 = true;
        } else {
            float t = (float)AnimTimer;
            if (t < Released) Timer = (long)(MaxTimer * (t / Released));
            else if (Timer > MaxTimer) Timer = MaxTimer;
        }
    }
};
int main()
{
    State s = { true, 8 };
    Berserk a = { true, &s, false, 5, 10.0f, 4, 100, true, false, true, 0, false };
    a.CancelMove();
    if (s.Active || s.St != 0 || !a.effectKilled || a.created != 1 || !a.removed84) { std::printf("0078c610_TEST FAIL\n"); return 1; }
    State s2 = { true, 3 };
    Berserk b = { true, &s2, false, 5, 10.0f, 4, 100, false, false, false, 0, false };
    b.CancelMove();                                              // 4 / 10 * 100 = 40
    if (b.Timer != 40 || b.removed84) { std::printf("0078c610_TEST FAIL\n"); return 1; }
    Berserk c = { true, &s2, true, 150, 10.0f, 20, 100, false, false, false, 0, false };
    c.CancelMove();                                              // clamp to max
    if (c.Timer != 100) { std::printf("0078c610_TEST FAIL\n"); return 1; }
    Berserk d = { false, 0, false, 7, 1.0f, 0, 1, false, false, false, 0, false };
    d.CancelMove();
    if (d.Timer != 7) { std::printf("0078c610_TEST FAIL\n"); return 1; }
    std::printf("0078c610_TEST PASS\n");
    return 0;
}