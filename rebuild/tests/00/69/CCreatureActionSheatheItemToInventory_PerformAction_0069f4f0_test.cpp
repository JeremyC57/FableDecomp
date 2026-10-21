#include <cstdio>
// Behaviour copy of CCreatureAction_SheatheItemToInventory::PerformAction (0x0069F4F0): drop the primary item (dousing a live flammable), refresh sheathed weapons, cancel the flourish, restart the mode, drop the main player's aggression.
struct Item { bool alive; bool flammable; bool burning; };
struct Player { bool main; bool aggressive; };
struct Creature {
    Item* primary; bool refreshed; bool flourishValid; bool restarted; Player* player;
    Item* RemoveThingFromPrimarySlot(bool) { Item* i = primary; primary = 0; return i; }
};
static void PerformAction(Creature& c)
{
    Item* item = c.RemoveThingFromPrimarySlot(false);
    if (item && item->alive && item->flammable) item->burning = false;
    c.refreshed = true;
    c.flourishValid = false;
    c.restarted = true;
    if (c.player->main) c.player->aggressive = false;
}
int main()
{
    Item torch = { true, true, true }; Player p = { true, true };
    Creature c = { &torch, false, true, false, &p };
    PerformAction(c);
    bool ok = c.primary == 0 && !torch.burning && c.refreshed && !c.flourishValid && c.restarted && !p.aggressive;
    Item dead = { false, true, true }; Player npc = { false, true }; Creature c2 = { &dead, false, true, false, &npc };
    PerformAction(c2);
    ok = ok && dead.burning && npc.aggressive && c2.restarted;
    std::printf(ok ? "0069f4f0_TEST PASS\n" : "0069f4f0_TEST FAIL\n");
    return ok ? 0 : 1;
}