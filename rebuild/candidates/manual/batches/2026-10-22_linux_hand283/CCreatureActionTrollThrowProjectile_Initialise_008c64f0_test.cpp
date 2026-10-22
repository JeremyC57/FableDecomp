// Behaviour test for CCreatureAction_TrollThrowProjectile::Initialise (0x008C64F0): the projectile is created from the
// ability's physical def, remembered and (with a carrying TC) put in the left or right throw slot by the weapon hand,
// then hidden; a 0..99 roll below the willpower-indicator chance copies its positive value into TC 0x30.
#include <cstdio>
#include <cstring>

struct Projectile { int defIndex; int player; const char* slot; bool visible; };
struct Troll { bool canCarry; int hand; int player; bool hasWillPower; float chance; int value; bool hasTC30; int tcValue; };

static unsigned int g_seed = 12345;
static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int Roll100()
{
    g_seed = g_seed * 0x24a1 + 0x24df;
    g_seed = Ror13(g_seed);
    return g_seed % 100;
}

static Projectile* Initialise(Troll* t, Projectile* storage, int projectileDef, bool createOk)
{
    Projectile* thing = createOk ? storage : 0;
    if (thing) {
        thing->defIndex = projectileDef;
        thing->player = t->player;
        thing->slot = 0;
        if (t->canCarry)
            thing->slot = t->hand == 1 ? "CARRY_SLOT_TROLL_THROW_ROCK_LEFT" : "CARRY_SLOT_TROLL_THROW_ROCK";
        thing->visible = false;
    }
    if (t->hasWillPower) {
        unsigned int roll = Roll100();
        if ((float)roll < t->chance && t->hasTC30 && t->value > 0)
            t->tcValue = t->value;
    }
    return thing;
}

int main()
{
    Troll t = { true, 1, 2, true, 100.0f, 7, true, 0 };
    Projectile p = { 0, 0, 0, true };
    if (Initialise(&t, &p, 42, true) != &p || p.defIndex != 42 || p.player != 2 || p.visible
        || std::strcmp(p.slot, "CARRY_SLOT_TROLL_THROW_ROCK_LEFT") != 0 || t.tcValue != 7) {
        std::printf("008c64f0_TEST FAIL\n"); return 1;
    }
    Troll u = { false, 0, 1, true, 0.0f, 9, true, 3 };
    Projectile q = { 0, 0, 0, true };
    if (Initialise(&u, &q, 5, true) != &q || q.slot != 0 || u.tcValue != 3) { std::printf("008c64f0_TEST FAIL\n"); return 1; }
    Troll w = { true, 0, 1, false, 100.0f, 9, true, 0 };
    if (Initialise(&w, &q, 5, false) != 0 || w.tcValue != 0) { std::printf("008c64f0_TEST FAIL\n"); return 1; }
    w.canCarry = true; w.hand = 0;
    Initialise(&w, &q, 6, true);
    if (std::strcmp(q.slot, "CARRY_SLOT_TROLL_THROW_ROCK") != 0) { std::printf("008c64f0_TEST FAIL\n"); return 1; }
    std::printf("008c64f0_TEST PASS\n");
    return 0;
}
