#include <cstdio>
// Behaviour test for CCombatAction_StabThing::Initialise @ 0x0087ECB0 (event post + flag on a live target).
struct Thing { bool dead; bool hasEvents; int events; int lastShooter; long flags; int id; };
struct Action { Thing* creature; Thing* target; int baseInits; };
static void Initialise(Action& a)
{
    Thing* t = a.target;
    if (t != 0 && !t->dead) {
        if (t->hasEvents) { ++t->events; t->lastShooter = a.creature->id; }
        t->flags |= 0x20;
    }
    ++a.baseInits;
}
static int Fail() { std::printf("0087ecb0_TEST FAIL\n"); return 1; }
int main()
{
    Thing hero = {false, false, 0, 0, 0, 7}, troll = {false, true, 0, 0, 0, 9};
    Action a = {&hero, &troll, 0};
    Initialise(a);
    if (troll.events != 1 || troll.lastShooter != 7 || troll.flags != 0x20 || a.baseInits != 1) return Fail();
    troll.dead = true; troll.flags = 0; Initialise(a);
    if (troll.events != 1 || troll.flags != 0 || a.baseInits != 2) return Fail();
    a.target = 0; Initialise(a); if (a.baseInits != 3) return Fail();
    troll.dead = false; troll.hasEvents = false; a.target = &troll; Initialise(a);
    if (troll.events != 1 || troll.flags != 0x20) return Fail();
    std::printf("0087ecb0_TEST PASS\n");
    return 0;
}