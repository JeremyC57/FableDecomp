// Behaviour test for CM_BanditTrigger::Main (0x00E1E960): unless the trader is not following or the trigger already
// fired, wait for the hero within range, wait until the spawn point is off screen, spawn a random bandit type there,
// give it the spawn point's item unless the data string is "" or "NULL", then mark fired and remove the trigger.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int heroNearAt; int offScreenAt;
    bool following; bool triggered; const char* data;
    int spawned; int items; int removed; int typeIndex; int randValue; int typeCount;
};

static bool Terminating(const Sim& s) { return s.frame >= s.terminateAt; }
static bool OnScreen(const Sim& s) { return s.frame < s.offScreenAt; }
static void Main(Sim& s)
{
    ++s.frame;
    if (Terminating(s)) return;
    if (!s.following) return;
    if (s.triggered) return;
    while (s.frame < s.heroNearAt) { ++s.frame; if (Terminating(s)) return; }
    if (Terminating(s)) return;
    for (;;) {
        while (OnScreen(s)) { ++s.frame; if (Terminating(s)) return; }
        int timer = 3;
        while (OnScreen(s) && timer != 0) { ++s.frame; --timer; if (Terminating(s)) return; }
        if (!OnScreen(s)) break;
        ++s.frame;
        if (Terminating(s)) return;
    }
    s.typeIndex = s.randValue % s.typeCount;
    ++s.spawned;
    if (std::strcmp(s.data, "") != 0 && std::strcmp(s.data, "NULL") != 0)
        ++s.items;
    s.triggered = true;
    ++s.removed;
}

int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.heroNearAt = 4; a.offScreenAt = 9; a.following = true; a.data = "BanditSword";
    a.randValue = 7; a.typeCount = 3;
    Main(a);
    bool ok = a.spawned == 1 && a.items == 1 && a.removed == 1 && a.triggered && a.typeIndex == 1 && a.frame == 9;

    Main(a);                                      // already triggered: nothing more
    ok = ok && a.spawned == 1 && a.removed == 1;

    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 100; b.following = true; b.data = "NULL"; b.typeCount = 2; b.randValue = 4;
    Main(b);
    ok = ok && b.spawned == 1 && b.items == 0 && b.typeIndex == 0;

    Sim c; std::memset(&c, 0, sizeof(c));
    c.terminateAt = 100; c.following = false; c.data = "";
    Main(c);
    ok = ok && c.spawned == 0 && !c.triggered;

    Sim d; std::memset(&d, 0, sizeof(d));
    d.terminateAt = 5; d.heroNearAt = 3; d.offScreenAt = 50; d.following = true; d.data = ""; d.typeCount = 1;
    Main(d);                                      // terminated while the spawn point is on screen
    ok = ok && d.spawned == 0 && !d.triggered;

    std::printf(ok ? "00e1e960_TEST PASS\n" : "00e1e960_TEST FAIL\n");
    return ok ? 0 : 1;
}
