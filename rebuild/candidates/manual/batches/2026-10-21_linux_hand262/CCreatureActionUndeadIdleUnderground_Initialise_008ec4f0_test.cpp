// behaviour test for 008ec4f0 CCreatureAction_UndeadIdleUnderground::Initialise
#include <cstdio>
#include <cstring>
struct Thing { bool dead; bool hasTrail; int trailsCleared; };
struct Creature {
    int mode; int slot28Arg; bool hasCarrying; Thing* carried[8]; int carriedCount; bool has9; bool flag10;
};
struct Action { Creature* creature; const char* anim; int baseInits; int guiRefresh; };
static void Initialise(Action& a)
{
    Creature& c = *a.creature;
    c.mode = 0x1e;
    ++a.guiRefresh;
    c.slot28Arg = 0;
    if (c.hasCarrying) {
        Thing* things[8];
        int count = c.carriedCount;
        for (int k = 0; k < count; ++k)
            things[k] = c.carried[k];
        for (int i = 0; i < count; ++i) {
            Thing* thing = things[i];
            if (thing && !thing->dead && thing->hasTrail)
                ++thing->trailsCleared;
        }
    }
    if (c.has9) c.flag10 = false;
    a.anim = "ST_IDLE";
    ++a.baseInits;
}
int main()
{
    int bad = 0;
    Thing t1 = { false, true, 0 }, t2 = { true, true, 0 }, t3 = { false, false, 0 };
    Creature c; c.mode = 0; c.slot28Arg = 5; c.hasCarrying = true; c.carriedCount = 4; c.carried[0] = &t1; c.carried[1] = 0;
    c.carried[2] = &t2; c.carried[3] = &t3; c.has9 = true; c.flag10 = true;
    Action a = { &c, 0, 0, 0 };
    Initialise(a);
    if (c.mode != 0x1e || c.slot28Arg != 0 || a.guiRefresh != 1) bad |= 1;
    if (t1.trailsCleared != 1 || t2.trailsCleared != 0 || t3.trailsCleared != 0) bad |= 2;   // dead / no trail skipped
    if (c.flag10 || std::strcmp(a.anim, "ST_IDLE") || a.baseInits != 1) bad |= 4;
    c.hasCarrying = false; c.has9 = false; c.flag10 = true;
    Initialise(a);
    if (t1.trailsCleared != 1 || !c.flag10 || a.baseInits != 2) bad |= 8;
    if (bad) { std::printf("008ec4f0_TEST FAIL\n"); return 1; }
    std::printf("008ec4f0_TEST PASS\n");
    return 0;
}
