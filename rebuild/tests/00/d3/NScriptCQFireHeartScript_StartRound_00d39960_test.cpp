// Behaviour test for 0x00D39960 NScript::CQ_FireHeartScript::StartRound: a standalone model of the round loop
// (info pages only the first time with the tutorial on; heartbeat fast/medium/slow by time left; the time warning once;
// at zero the hero is zapped and the next StartRound is spawned; the HUD clock is removed at the end).
#include <cstdio>
#include <cstring>

struct Model {
    bool infoDisplayed, tutorial, roundEnded, preventRoundEnd, comment[5];
    long timeLimit, warnTime;
    int infoPages, fast, medium, slow, warnings, zapped, spawned, clocksRemoved;
};

static void StartRound(Model& m)
{
    m.roundEnded = false;
    if (!m.infoDisplayed) {
        if (m.tutorial) m.infoPages += 3;
        m.infoDisplayed = true;
    }
    for (long i = 0; i < 5; ++i) m.comment[i] = false;
    long roundTime = m.timeLimit, beat = 3;
    while (roundTime >= 0 && !m.roundEnded) {
        --roundTime; --beat;                       // one "second" per frame in the model
        if (beat <= 0) {
            if (roundTime <= 10) { ++m.fast; beat = 1; }
            else if (roundTime <= 20) { ++m.medium; beat = 2; }
            else { ++m.slow; beat = 3; }
        }
        if (roundTime <= m.warnTime && !m.comment[4]) { ++m.warnings; m.comment[4] = true; }
        else if (roundTime == 0 && !m.roundEnded && !m.preventRoundEnd) { m.roundEnded = true; ++m.zapped; ++m.spawned; }
    }
    ++m.clocksRemoved;
}

int main()
{
    int fails = 0;
    Model m; std::memset(&m, 0, sizeof m);
    m.tutorial = true; m.timeLimit = 30; m.warnTime = 15;
    StartRound(m);
    if (m.infoPages != 3 || m.warnings != 1 || m.zapped != 1 || m.spawned != 1 || m.clocksRemoved != 1) ++fails;
    if (m.slow == 0 || m.medium == 0 || m.fast == 0) ++fails;
    StartRound(m);
    if (m.infoPages != 3 || m.warnings != 2 || m.zapped != 2) ++fails;
    Model p; std::memset(&p, 0, sizeof p);
    p.timeLimit = 5; p.warnTime = 2; p.preventRoundEnd = true;
    StartRound(p);
    if (p.infoPages != 0 || !p.infoDisplayed || p.zapped != 0 || p.warnings != 1 || p.clocksRemoved != 1) ++fails;
    if (fails) { std::printf("00d39960_TEST FAIL\n"); return 1; }
    std::printf("00d39960_TEST PASS\n");
    return 0;
}