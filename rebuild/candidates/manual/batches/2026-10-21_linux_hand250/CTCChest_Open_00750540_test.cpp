// Behaviour test for CTCChest::Open (0x00750540): open once, persist on demand, spawn and attach the open particle.
#include <cstdio>
struct Def { bool persist; long effect; };
struct Chest {
    bool open, opening, closing, opened, closed, forced, persisted, pushable; long spawned; bool attached; Def* def;
    void Open()
    {
        if (open) return;
        if (def) {
            open = true; opening = false; closing = false; opened = true; closed = false;
            forced = true;
            if (def->persist) persisted = true;
            if (def->effect > 0) { spawned = def->effect; attached = true; }
            pushable = false;
        }
    }
};
int main()
{
    Def d = { true, 7 };
    Chest c = { false, true, true, false, true, false, false, true, 0, false, &d };
    c.Open();
    if (!c.open || c.opening || !c.opened || c.closed || !c.forced || !c.persisted || c.pushable || c.spawned != 7 || !c.attached)
    { std::printf("00750540_TEST FAIL\n"); return 1; }
    c.spawned = 0; c.Open();
    if (c.spawned != 0) { std::printf("00750540_TEST FAIL\n"); return 1; }
    Chest e = { false, false, false, false, false, false, false, true, 0, false, 0 }; e.Open();
    if (e.open || !e.pushable) { std::printf("00750540_TEST FAIL\n"); return 1; }
    std::printf("00750540_TEST PASS\n");
    return 0;
}
