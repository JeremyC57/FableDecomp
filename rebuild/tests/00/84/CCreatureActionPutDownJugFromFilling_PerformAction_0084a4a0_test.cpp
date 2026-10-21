#include <cstdio>
// Behaviour copy of CCreatureAction_PutDownJugFromFilling::PerformAction (0x0084A4A0): remove the jug from its carry slot, reset its physics when alive, drop mode 0x2b, mark it filled.
struct Def { long refs; long FillCarrySlot; };
struct Physics { long movement; };
struct Jug { bool alive; Def* def; Physics physics; unsigned char Filled; };
struct Carrying { long removedSlot; bool removedNow; void RemoveThingInCarrySlot(long s, bool now) { removedSlot = s; removedNow = now; } };
struct Modes { unsigned long removed; void RemoveMode(unsigned long m) { removed = m; } };
struct Action {
    Carrying* carrying; Modes* modes; Jug* jug;
    void PerformAction() {
        Def* d = jug->def; ++d->refs;                 // GetSubDef hands over a counted reference
        carrying->RemoveThingInCarrySlot(d->FillCarrySlot, true);
        if (jug && jug->alive) jug->physics.movement = 0;
        modes->RemoveMode(0x2b);
        jug->Filled = 1;
        --d->refs;                                    // the CDefPointer local releases it
    }
};
int main()
{
    Def d = { 1, 6 }; Jug j = { true, &d, { 3 }, 0 }; Carrying c = { -1, false }; Modes m = { 0 };
    Action a = { &c, &m, &j };
    a.PerformAction();
    bool ok = c.removedSlot == 6 && c.removedNow && j.physics.movement == 0 && m.removed == 0x2b && j.Filled == 1 && d.refs == 1;
    Jug dead = { false, &d, { 3 }, 0 }; a.jug = &dead; a.PerformAction();
    ok = ok && dead.physics.movement == 3 && dead.Filled == 1;
    std::printf(ok ? "0084a4a0_TEST PASS\n" : "0084a4a0_TEST FAIL\n");
    return ok ? 0 : 1;
}