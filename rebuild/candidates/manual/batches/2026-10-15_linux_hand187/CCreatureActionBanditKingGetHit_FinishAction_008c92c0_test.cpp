#include <cstdio>
#include <cstring>
// Behaviour test for CCreatureAction_BanditKingGetHit::FinishAction @ 0x008C92C0 (response selection).
struct Thing { bool dead; unsigned char flags6c; int facing; };
struct Creature { int facing; const char* anim; long a, b; int actions; };
static bool IsHitFromBehind(Creature* c, Thing* t) { return c->facing == t->facing; }
static int g_base = 0;
static void FinishAction(Creature* creature, Thing* attacker)
{
    ++g_base;
    if (attacker && !attacker->dead && (attacker->flags6c & 8)) {
        if (IsHitFromBehind(creature, attacker)) { creature->anim = "REAR_ATTACK"; creature->a = 0; creature->b = 1; }
        else { creature->anim = "STRIKE_MEDIUM_FRONT"; creature->a = 1; creature->b = 0; }
        ++creature->actions;
    }
}
static int Fail() { std::printf("008c92c0_TEST FAIL\n"); return 1; }
int main()
{
    Creature c = {1, "", 0, 0, 0}; Thing t = {false, 8, 1};
    FinishAction(&c, &t); if (std::strcmp(c.anim, "REAR_ATTACK") != 0 || c.a != 0 || c.b != 1) return Fail();
    t.facing = 2; FinishAction(&c, &t); if (std::strcmp(c.anim, "STRIKE_MEDIUM_FRONT") != 0 || c.a != 1 || c.actions != 2) return Fail();
    t.flags6c = 0; FinishAction(&c, &t); if (c.actions != 2) return Fail();
    t.flags6c = 8; t.dead = true; FinishAction(&c, &t); FinishAction(&c, 0); if (c.actions != 2 || g_base != 5) return Fail();
    std::printf("008c92c0_TEST PASS\n");
    return 0;
}
