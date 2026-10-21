#include <cstdio>
// Behaviour copy of CCreatureAction_OpenChest::PerformAction (0x008448E0): open the chest, stop the creature, queue an opened-chest event.
struct Chest { bool open; void Open() { open = true; } };
struct Thing { bool alive; Chest chest; };
struct Events { int count; const Thing* lastChest; void AddEvent(const Thing* chest) { ++count; lastChest = chest; } };
struct Mover { long stopMode; bool stopped; void Stop(long m) { stopMode = m; stopped = true; } };
struct Action {
    Thing* chest; Mover* mover; Events* events;
    void PerformAction() {
        if (chest && chest->alive) { chest->chest.Open(); mover->Stop(0); events->AddEvent(chest); }
    }
};
int main()
{
    Thing c = { true, { false } }; Mover m = { 7, false }; Events e = { 0, 0 };
    Action a = { &c, &m, &e };
    a.PerformAction();
    bool ok = c.chest.open && m.stopped && m.stopMode == 0 && e.count == 1 && e.lastChest == &c;
    Thing dead = { false, { false } }; a.chest = &dead; a.PerformAction();
    ok = ok && !dead.chest.open && e.count == 1;
    std::printf(ok ? "008448e0_TEST PASS\n" : "008448e0_TEST FAIL\n");
    return ok ? 0 : 1;
}