// behaviour test for 0x0084A0A0 CCreatureAction_PutDownThingAtTable::PerformAction(): take the primary-slot thing out
// (true), switch a live thing's physics off, and drop the action's creature mode when one is set
#include <cstdio>
struct Physics { int enables; bool last; void EnablePhysics(bool e) { ++enables; last = e; } };
struct Thing { bool dead; Physics* PhysicsTC; bool IsDead() const { return dead; } };
struct Carrying { Thing* primary; int removed; bool arg;
    Thing* GetThingInPrimarySlot() const { return primary; }
    Thing* RemoveThingFromPrimarySlot(bool b) { ++removed; arg = b; Thing* t = primary; primary = 0; return t; } };
struct Modes { unsigned long last; int count; void RemoveMode(unsigned long m) { last = m; ++count; } };
struct Action {
    Carrying* carrying; Modes* modes; long Mode;
    void PerformAction()
    {
        Thing* thing = carrying->GetThingInPrimarySlot();
        carrying->RemoveThingFromPrimarySlot(true);
        if (thing && !thing->IsDead())
            thing->PhysicsTC->EnablePhysics(false);
        if (Mode)
            modes->RemoveMode(Mode);
    }
};
int main()
{
    int fails = 0;
    Physics ph = { 0, true };
    Thing mug = { false, &ph };
    Carrying car = { &mug, 0, false };
    Modes m = { 0, 0 };
    Action a = { &car, &m, 9 };
    a.PerformAction();
    if (car.removed != 1 || !car.arg || ph.enables != 1 || ph.last || m.count != 1 || m.last != 9) fails |= 1;
    a.Mode = 0; a.PerformAction();
    if (car.removed != 2 || ph.enables != 1 || m.count != 1) fails |= 2;
    mug.dead = true; car.primary = &mug; a.PerformAction();
    if (ph.enables != 1) fails |= 4;
    if (fails) { std::printf("0084a0a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0084a0a0_TEST PASS\n");
    return 0;
}