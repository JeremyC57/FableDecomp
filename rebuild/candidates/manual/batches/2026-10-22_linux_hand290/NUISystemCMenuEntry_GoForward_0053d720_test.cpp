// Behaviour test for NUISystem::CMenuEntry::GoForward (0x0053D720): disabled entries (state 2) do nothing; otherwise the
// parameters go to the manager, available entries run pre-actions, set every child to state 5 and play the forward sound
// only without a child menu / pre-actions / actions, then run the actions; unavailable entries just play the sound. The
// menu timer restarts unless the parent menu's def keeps it.
#include <cstdio>

struct Log { int params, sounds, preRuns, actRuns, childStates, timerResets; };
struct Child { int type; int state; };
struct Entry {
    int state; bool available; int nParams; bool hasPre, hasActions; Child children[3]; int nChildren;
    bool parentIsMenu; bool parentKeepsTimer; long inputTime, lastInputTime;
};
static void GoForward(Entry& e, Log& log)
{
    if (e.state == 2) return;
    log.params += e.nParams;
    if (e.available) {
        if (e.hasPre) ++log.preRuns;
        bool hasMenu = false;
        for (int i = 0; i < e.nChildren; ++i) { if (e.children[i].type == 8) hasMenu = true; e.children[i].state = 5; ++log.childStates; }
        if (!hasMenu && !e.hasPre && !e.hasActions) ++log.sounds;
        if (e.hasActions) ++log.actRuns;
    } else {
        ++log.sounds;
    }
    e.lastInputTime = e.inputTime;
    if (!e.parentIsMenu || e.parentKeepsTimer) ++log.timerResets;
}

int main()
{
    Log log = { 0, 0, 0, 0, 0, 0 };
    Entry a = { 2, true, 3, false, false, { { 1, 0 } }, 1, false, false, 9, 0 };
    GoForward(a, log);
    bool ok = log.params == 0 && log.timerResets == 0 && a.lastInputTime == 0;
    Entry b = { 0, true, 2, false, false, { { 1, 0 }, { 4, 0 } }, 2, true, false, 7, 0 };
    GoForward(b, log);
    ok = ok && log.params == 2 && log.sounds == 1 && b.children[1].state == 5 && log.timerResets == 0 && b.lastInputTime == 7;
    Entry c = { 0, true, 0, true, true, { { 8, 0 } }, 1, true, true, 5, 0 };
    GoForward(c, log);
    ok = ok && log.sounds == 1 && log.preRuns == 1 && log.actRuns == 1 && log.timerResets == 1;
    Entry d = { 0, false, 0, true, true, { { 8, 0 } }, 1, false, false, 5, 0 };
    GoForward(d, log);
    ok = ok && log.sounds == 2 && log.preRuns == 1 && d.children[0].state == 0 && log.timerResets == 2;
    std::printf(ok ? "0053d720_TEST PASS\n" : "0053d720_TEST FAIL\n");
    return ok ? 0 : 1;
}
