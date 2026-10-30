// Behaviour test for 0x0087EBD0 CCombatAction_KickThingOnGround::FinishAction (see src/0087ebd0.cpp): while the kicked
// thing is alive, the kicker's solid-to-thing component (0x4D) forgets the target and the target's forgets the kicker
// (each only when present); the base FinishAction always runs.
#include <cstdio>
struct Thing;
struct Solid { Thing* Removed; int Calls; void RemoveThingNotSolidTo(Thing* t) { Removed = t; ++Calls; } };
struct Thing { bool Alive; Solid* Component; };
struct Action {
    Thing* Creature; Thing* Target; int BaseCalls;
    void FinishAction()
    {
        if (Target && Target->Alive) {
            if (Creature->Component) Creature->Component->RemoveThingNotSolidTo(Target);
            if (Target->Component) Target->Component->RemoveThingNotSolidTo(Creature);
        }
        ++BaseCalls;
    }
};
int main()
{
    Solid ks = { 0, 0 }, ts = { 0, 0 };
    Thing kicker = { true, &ks }, target = { true, &ts };
    Action a = { &kicker, &target, 0 };
    a.FinishAction();
    bool ok = ks.Removed == &target && ts.Removed == &kicker && a.BaseCalls == 1;
    target.Alive = false;
    a.FinishAction();
    ok = ok && ks.Calls == 1 && ts.Calls == 1 && a.BaseCalls == 2;
    target.Alive = true; kicker.Component = 0;
    a.FinishAction();
    ok = ok && ts.Calls == 2 && a.BaseCalls == 3;
    std::printf(ok ? "0087ebd0_TEST PASS\n" : "0087ebd0_TEST FAIL\n");
    return ok ? 0 : 1;
}