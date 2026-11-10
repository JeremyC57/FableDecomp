// Behaviour test for CTCInventoryTrade::ConstructBuyActionsList @ 0x005A3E47: the buyable quantity (stock count capped by the room in the
// hero's inventory, stepped down until affordable), the entry count (1..4), the extra "10" / "50" / "all" entries (not in barber,
// tattoo or title shops), and the attach decision (affordable for one, space or special case, count != 0).
#include <cstdio>
#include <vector>
struct Shop { bool barber, tattoo, title; };
struct Result { const char* list; long quantity; long itemCount; std::vector<unsigned long> amounts; bool attached; };
static long gMoney, gPrice, gRoom; static bool gSpecial;
static bool IsItemAffordable(long, long n) { return gPrice * n <= gMoney; }
static Result Construct(long count, const Shop& shop)
{
    Result r; r.list = "UI_LIST_BUY_ACTIONS";
    long maxBuyable = gRoom;
    long quantity = count;
    if (maxBuyable <= count)
        quantity = maxBuyable;
    for (; quantity >= 1; --quantity)
        if (IsItemAffordable(0, quantity))
            break;
    r.quantity = quantity;
    r.itemCount = quantity > 50 ? 4 : quantity > 10 ? 3 : quantity > 1 ? 2 : 1;
    bool plain = !shop.barber && !shop.tattoo && !shop.title;
    if (quantity > 10 && plain) r.amounts.push_back(10);
    if (quantity > 50 && plain) r.amounts.push_back(50);
    if (quantity > 1 && plain) r.amounts.push_back((unsigned long)quantity);
    r.attached = IsItemAffordable(0, 1) && (gRoom > 0 || gSpecial) && count != 0;
    return r;
}
int main()
{
    int ok = 1;
    Shop normal = { false, false, false }, barber = { true, false, false };
    gMoney = 1000; gPrice = 10; gRoom = 200; gSpecial = false;
    Result a = Construct(80, normal);            // 80 in stock, room 200, affordable 100 -> 80
    if (a.quantity != 80 || a.itemCount != 4 || a.amounts.size() != 3 || a.amounts[2] != 80 || !a.attached) ok = 0;
    gMoney = 250;                                 // affordable 25
    Result b = Construct(80, normal);
    if (b.quantity != 25 || b.itemCount != 3 || b.amounts.size() != 2 || b.amounts[0] != 10 || b.amounts[1] != 25) ok = 0;
    gRoom = 5;                                    // capped by the room
    Result c = Construct(80, barber);
    if (c.quantity != 5 || c.itemCount != 2 || !c.amounts.empty() || !c.attached) ok = 0;
    gRoom = 0; gSpecial = false;                  // no room, not special: not attached
    Result d = Construct(3, normal);
    if (d.quantity != 0 || d.itemCount != 1 || d.attached) ok = 0;
    gSpecial = true;
    if (!Construct(3, normal).attached || Construct(0, normal).attached) ok = 0;
    gMoney = 5; gRoom = 10;                       // cannot afford one
    if (Construct(3, normal).attached) ok = 0;
    if (!ok) { std::printf("005a3e47_TEST FAIL\n"); return 1; }
    std::printf("005a3e47_TEST PASS\n");
    return 0;
}
