#include <cstdio>
// Behaviour model of CInputProcessCombat::ProcessArmedMeleeInput (0x0068ECA0): which game event a pressed action queues.
enum { NONE = 0, FLOURISH = 1, BREAK_BLOCK = 2, EVT20 = 3, ATTACK = 4, EVT22 = 5 };
static int Process(bool pressed, int action, bool flourishValid, bool breakBlockValid)
{
    if (!pressed) return NONE;
    if (action == 8 && flourishValid) return FLOURISH;
    if (action == 8 && breakBlockValid) return BREAK_BLOCK;
    if (action == 0xf) return EVT20;
    if (action == 0xc) return ATTACK;
    if (action == 0xe) return EVT22;
    return NONE;
}
int main()
{
    int ok = Process(false, 8, true, true) == NONE;
    ok = ok && Process(true, 8, true, true) == FLOURISH;
    ok = ok && Process(true, 8, false, true) == BREAK_BLOCK;
    ok = ok && Process(true, 8, false, false) == NONE;
    ok = ok && Process(true, 0xf, false, false) == EVT20;
    ok = ok && Process(true, 0xc, true, true) == ATTACK;
    ok = ok && Process(true, 0xe, true, true) == EVT22;
    ok = ok && Process(true, 3, true, true) == NONE;
    if (!ok) { std::printf("0068eca0_TEST FAIL\n"); return 1; }
    std::printf("0068eca0_TEST PASS\n");
    return 0;
}