#include <cstdio>
// Behaviour test for CCreatureAction_BanditKingStuckGetHit::FinishAction @ 0x008C91E0.
struct Thing { bool dead; int kills; };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } void SetPItem(Thing* t) { item = t; } };
struct Creature { long lastCode; bool lastStarted; int actions; };
static int g_base = 0;
static void FinishAction(Creature* creature, Ptr& stuck)
{
    ++g_base;
    creature->lastCode = 150; creature->lastStarted = true; ++creature->actions;   // CCreatureAction_BanditKingUnstuck
    if (stuck.GetPItem() != 0 && !stuck.GetPItem()->dead) {
        ++stuck.GetPItem()->kills;
        Ptr none; none.SetPItem(0);
        stuck.SetPItem(none.GetPItem());
    }
}
static int Fail() { std::printf("008c91e0_TEST FAIL\n"); return 1; }
int main()
{
    Creature c = {0, false, 0}; Thing sword = {false, 0}; Ptr p; p.item = &sword;
    FinishAction(&c, p);
    if (sword.kills != 1 || p.item != 0 || c.actions != 1 || c.lastCode != 150 || !c.lastStarted) return Fail();
    sword.dead = true; p.item = &sword; FinishAction(&c, p);
    if (sword.kills != 1 || p.item != &sword || c.actions != 2 || g_base != 2) return Fail();
    std::printf("008c91e0_TEST PASS\n");
    return 0;
}