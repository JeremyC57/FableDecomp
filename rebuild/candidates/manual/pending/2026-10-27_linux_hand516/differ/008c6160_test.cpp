// behaviour test for 0x008C6160 CCreatureAction_RockTrollFallToDeath::Initialise: FALL_TO_DEATH, AI suspended for an AI creature, then base Initialise
#include <cstdio>
#include <cstring>
struct Creature { unsigned char type; bool suspended; };
struct Action { Creature* creature; char anim[32]; int baseInits; };
static void Initialise(Action& a)
{
    std::strcpy(a.anim, "FALL_TO_DEATH");
    if (a.creature->type == 1) a.creature->suspended = true;
    a.baseInits++;
}
int main()
{
    Creature c = { 1, false }; Action a; a.creature = &c; a.baseInits = 0;
    Initialise(a);
    if (!c.suspended || std::strcmp(a.anim, "FALL_TO_DEATH") || a.baseInits != 1) { std::printf("008c6160_TEST FAIL\n"); return 1; }
    Creature d = { 2, false }; a.creature = &d; Initialise(a);
    if (d.suspended || a.baseInits != 2) { std::printf("008c6160_TEST FAIL\n"); return 1; }
    std::printf("008c6160_TEST PASS\n");
    return 0;
}
