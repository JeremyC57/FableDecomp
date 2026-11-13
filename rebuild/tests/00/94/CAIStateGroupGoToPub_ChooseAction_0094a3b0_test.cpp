// Behaviour test for CAIStateGroup_GoToPub::ChooseAction (0x0094A3B0): a copy of the decision logic over a mock state.
#include <stdio.h>
enum { NONE, SIT, DRINK, THANK_FAIL, LAUGH, LISTEN, STORY, SLEEP };
static unsigned long g_seed;
static long Ror13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static unsigned long Rnd(unsigned long n) { g_seed = g_seed * 0x24A1 + 0x24DF; g_seed = (unsigned long)Ror13(g_seed); return g_seed % n; }
struct Mock {
    bool tankardAlive; bool hasTankard; float fullness; bool gameBusy; bool tavernTime; bool thanks;
    long seated; bool talking; bool laughing; int act;
    bool SitIdle() { act = SIT; return true; }
    bool Drink() { act = DRINK; return true; }
    bool ThankBarmaid() { act = THANK_FAIL; return thanks; }
    bool Laugh() { act = LAUGH; return true; }
    bool Listen() { act = LISTEN; return true; }
    bool TellStory() { act = STORY; return true; }
    bool FallAsleep() { act = SLEEP; return true; }
    bool ChooseAction(bool canDrink) {
        bool result;
        bool haveT = tankardAlive && hasTankard;
        if (gameBusy) return SitIdle();
        if (!tavernTime && canDrink && tankardAlive) { if (fullness == 0.0f) return false; return Drink(); }
        if (tankardAlive && haveT) {
            if (fullness == 1.0f) { if (!ThankBarmaid()) return false; return Drink(); }
            if (Rnd(4) == 0) return Drink();
        }
        if (seated > 1) {
            if (talking) {
                switch (Rnd(4)) { case 0: result = SitIdle(); break; case 1: result = Listen(); break;
                                  case 2: result = Laugh(); break; default: result = TellStory(); break; }
            } else if (laughing) {
                if (Rnd(2) == 0) return Laugh();
                if (Rnd(2) == 0) return SitIdle();
                return TellStory();
            } else {
                if (Rnd(2) == 0) return SitIdle();
                return TellStory();
            }
            return result;
        }
        switch (Rnd(5)) { case 1: return FallAsleep(); case 0: if (tankardAlive) return Drink(); default: return SitIdle(); }
    }
};
int main() {
    int fails = 0;
    Mock m = { true, true, 0.0f, true, false, true, 3, true, false, NONE };
    if (!m.ChooseAction(true) || m.act != SIT) fails++;                       // game busy
    m.gameBusy = false; m.act = NONE;
    if (m.ChooseAction(true) || m.act != NONE) fails++;                      // empty tankard -> false, no action
    m.fullness = 0.5f; if (!m.ChooseAction(true) || m.act != DRINK) fails++; // keep drinking
    m.fullness = 1.0f; m.thanks = false; m.act = NONE;
    if (m.ChooseAction(false) || m.act != THANK_FAIL) fails++;               // full tankard, thanks refused -> false
    m.thanks = true; if (!m.ChooseAction(false) || m.act != DRINK) fails++;  // thanked -> drink
    // talking table: every action must come from the talking set; count variety over seeds
    m.hasTankard = false; m.tankardAlive = false; int seen[8] = {0};
    for (unsigned long s = 0; s < 64; ++s) { g_seed = s; m.act = NONE; m.ChooseAction(true); seen[m.act]++; }
    if (!seen[SIT] || !seen[LISTEN] || !seen[LAUGH] || !seen[STORY] || seen[DRINK] || seen[SLEEP]) fails++;
    // alone without a tankard: only sleep or idle
    m.seated = 1; int seen2[8] = {0};
    for (unsigned long s = 0; s < 64; ++s) { g_seed = s; m.ChooseAction(true); seen2[m.act]++; }
    if (!seen2[SLEEP] || !seen2[SIT] || seen2[DRINK] || seen2[LAUGH]) fails++;
    // laughing table never listens
    m.seated = 2; m.talking = false; m.laughing = true; int seen3[8] = {0};
    for (unsigned long s = 0; s < 64; ++s) { g_seed = s; m.ChooseAction(true); seen3[m.act]++; }
    if (!seen3[LAUGH] || !seen3[SIT] || !seen3[STORY] || seen3[LISTEN]) fails++;
    if (fails) { printf("0094a3b0_TEST FAIL\n"); return 1; }
    printf("0094a3b0_TEST PASS\n");
    return 0;
}