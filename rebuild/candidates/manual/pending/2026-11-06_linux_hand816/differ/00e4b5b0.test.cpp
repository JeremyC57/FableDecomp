// Behaviour test for NScript::CV_BanditTollScript::CTollBandit::Main (0x00E4B5B0): the start-state fix-up (angered bandits go to 6; an
// unpaid 5 or an un-angered 6 restarts at 2) and the state-2 fee answer (no money -> 3, otherwise pay -> 4). A model of the state rules.
#include <cstdio>
static int StartState(int state, bool angered, bool tollPaid)
{
    if (angered) state = 6;
    if ((state == 5 && !tollPaid) || (!angered && state == 6)) state = 2;
    return state;
}
static int AskToll(long heroGold, long fee) { return heroGold < fee ? 3 : 4; }
int main()
{
    bool ok = StartState(1, false, false) == 1 && StartState(1, true, false) == 6 && StartState(5, false, false) == 2 && StartState(5, false, true) == 5
        && StartState(6, false, false) == 2 && StartState(6, true, true) == 6;
    ok = ok && AskToll(10, 50) == 3 && AskToll(50, 50) == 4 && AskToll(80, 50) == 4;
    std::printf(ok ? "00e4b5b0_TEST PASS\n" : "00e4b5b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
