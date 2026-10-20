// behaviour test for CAIStateGroup_OpenAndCloseShop (0x00926b30): the shared shop-sync logic over plain stubs
#include <cstdio>
struct Shop { bool opening; };
struct Keeper { Shop* shop; };
struct Sign { bool closed; };
struct Building { unsigned char flags91; unsigned char flagsD4; int locks; bool lockedState; Sign* sign; };
struct Creature { Building* shop; Keeper* keeper; };
struct Group {
    Creature* creature;
    void Update()
    {
        Building* shop = creature->shop;
        if (shop && !(shop->flags91 & 1)) {
            Building* b = creature->shop;
            b->flagsD4 |= 1;
            bool open = creature->keeper && creature->keeper->shop && creature->keeper->shop->opening;
            creature->shop->locks++; creature->shop->lockedState = !open;
            if (creature->shop->sign) creature->shop->sign->closed = !open;
        }
    }
    void InitStateGroup() { Update(); }
    bool InitCreaturePosition() { Update(); return false; }
};
int main()
{
    Shop s = { true }; Keeper k = { &s }; Sign sign = { true };
    Building b = { 0, 0x80, 0, true, &sign };
    Creature c = { &b, &k };
    Group g = { &c };
    bool r = g.InitCreaturePosition();
    bool ok = !r && b.flagsD4 == 0x81 && b.locks == 1 && !b.lockedState && !sign.closed;
    s.opening = false; g.InitStateGroup();
    if (!b.lockedState || !sign.closed) ok = false;
    c.keeper = 0; s.opening = true; g.InitStateGroup();
    if (!b.lockedState || b.locks != 3) ok = false;                 // no keeper component: closed
    b.flags91 = 1; g.InitStateGroup();
    if (b.locks != 3) ok = false;                                    // dead shop: untouched
    if (!ok) { std::printf("00926b30_TEST FAIL\n"); return 1; }
    std::printf("00926b30_TEST PASS\n");
    return 0;
}
