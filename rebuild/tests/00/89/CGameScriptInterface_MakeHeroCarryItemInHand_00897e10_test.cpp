// Behaviour test for 00897e10 CGameScriptInterface::MakeHeroCarryItemInHand: empty name empties the hand, else first live item of the def from inventory then the second store.
#include <cstdio>
#include <cstring>
struct Item { long def; bool dead; };
struct Hero { bool dead; bool carrying; Item* inv; int nInv; Item* store; int nStore; Item* inHand; };
static Item* Find(Item* items, int n, long def) { for (int i = 0; i < n; ++i) if (items[i].def == def) return &items[i]; return 0; }
static void Carry(Hero* hero, const char* name, long index)
{
    if (!hero || hero->dead) return;
    if (!std::strcmp(name, "")) { if (hero->carrying) hero->inHand = 0; return; }
    if (index <= 0) return;
    Item* item = Find(hero->inv, hero->nInv, index);
    if (!item || item->dead) item = Find(hero->store, hero->nStore, index);
    if (item && !item->dead) hero->inHand = item;
}
int main()
{
    Item inv[2] = { { 5, true }, { 6, false } }, store[1] = { { 5, false } };
    Hero h = { false, true, inv, 2, store, 1, 0 };
    Carry(&h, "SWORD", 5); bool a = h.inHand == &store[0];
    Carry(&h, "AXE", 6); bool b = h.inHand == &inv[1];
    Carry(&h, "", 0); bool c = h.inHand == 0;
    Carry(&h, "NONE", 0); bool d = h.inHand == 0;
    if (!a || !b || !c || !d) { std::printf("00897e10_TEST FAIL\n"); return 1; }
    std::printf("00897e10_TEST PASS\n");
    return 0;
}