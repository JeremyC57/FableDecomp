// Behaviour test for CHeroBoastsScript::BoastHobbeCaveInnocentSacrificed (0x00CE4C50): with the sacrifice state non-zero
// the thread waits for the quest's end (completed with state 2 = boast 22 completed, otherwise failed); once the state is
// 0 it shows the failed-sacrifice text (unless the quest failed), waits for the click and fails the boast.
#include <cstdio>
#include <cstring>

struct Game {
    int frame, failAt, completeAt, clickAt, terminateAt;
    int boastCompleted, boastFailed, infoShown;
    long* state; int stateZeroAt;
    bool Terminating() const { return terminateAt >= 0 && frame >= terminateAt; }
    bool Failed() const { return failAt >= 0 && frame >= failAt; }
    bool Completed() const { return completeAt >= 0 && frame >= completeAt; }
    bool Clicked() const { return clickAt >= 0 && frame >= clickAt; }
    void NewFrame() { ++frame; if (stateZeroAt >= 0 && frame >= stateZeroAt) *state = 0; }
};

static void Boast(Game& g)
{
    while (!g.Terminating()) {
        if (!g.Failed() && *g.state != 0) {
            if (g.Completed() && *g.state == 2) { if (g.Terminating()) return; g.boastCompleted = 22; return; }
            if (g.Completed()) { if (g.Terminating()) return; g.boastFailed = 22; return; }
            g.NewFrame();
            continue;
        }
        if (g.Terminating()) return;
        if (!g.Failed()) {
            if (g.Terminating()) return;
            g.infoShown = 1;
            while (!g.Clicked()) { g.NewFrame(); if (g.Terminating()) return; }
            if (g.Terminating()) return;
        }
        g.boastFailed = 22;
        return;
    }
}

static Game make(long* state)
{
    Game g; std::memset(&g, 0, sizeof(g));
    g.failAt = g.completeAt = g.clickAt = g.terminateAt = g.stateZeroAt = -1;
    g.state = state;
    return g;
}

int main()
{
    bool ok = true;
    long s = 2; Game a = make(&s); a.completeAt = 3; Boast(a);
    ok = ok && a.boastCompleted == 22 && a.boastFailed == 0 && a.frame == 3;
    s = 1; Game b = make(&s); b.completeAt = 2; Boast(b);
    ok = ok && b.boastFailed == 22 && b.boastCompleted == 0;
    s = 1; Game c = make(&s); c.stateZeroAt = 2; c.clickAt = 5; Boast(c);
    ok = ok && c.infoShown == 1 && c.boastFailed == 22 && c.frame == 5;
    s = 1; Game d = make(&s); d.failAt = 1; Boast(d);
    ok = ok && d.infoShown == 0 && d.boastFailed == 22 && d.frame == 1;
    s = 0; Game e = make(&s); e.terminateAt = 2; Boast(e);
    ok = ok && e.infoShown == 1 && e.boastFailed == 0 && e.frame == 2;
    std::printf(ok ? "00ce4c50_TEST PASS\n" : "00ce4c50_TEST FAIL\n");
    return 0;
}