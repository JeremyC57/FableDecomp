#include <cstdio>
#include <cstdlib>
#include <map>
#include <list>
// Model of CTCWife::FindAppropriateGift's selection logic (0x006CF530): budget from the price and a scale, then the closest-priced
// non-gift stock item of each eligible shop building.
struct Item { long key; long price; bool hasGift; long giftType; bool ring; };
struct Building { bool live; bool hasDef; long property; bool shop; std::list<Item> stock; };
struct Wife { long GiftGivingPriceValue; long GiftToGiveDef; };
static void FindAppropriateGift(Wife& w, std::list<Building>& village, float r, bool up)
{
    float spread = r * r + 1.0f;
    long budget = w.GiftGivingPriceValue;
    budget = (long)(up ? budget * spread : budget / spread);
    long bestGift = 0, bestValue = 0x7FFFFFFF;
    for (std::list<Building>::iterator b = village.begin(); b != village.end(); ++b) {
        if (!b->live || !b->hasDef) continue;
        if ((b->property & 0x10) && b->shop) {
            for (std::list<Item>::iterator it = b->stock.begin(); it != b->stock.end(); ++it) {
                if (!it->hasGift || (it->giftType != 1 && !it->ring)) {
                    long cost = it->price;
                    if (labs(cost - budget) < labs(budget - bestValue) && cost - budget < budget / 4) { bestValue = cost; bestGift = it->key; }
                }
            }
        }
    }
    if (bestGift > 0) { w.GiftToGiveDef = bestGift; w.GiftGivingPriceValue -= bestValue; }
}
int main()
{
    std::list<Building> v;
    Building shop; shop.live = true; shop.hasDef = true; shop.property = 0x10; shop.shop = true;
    Item a = { 5, 90, false, 0, false };   // close to 100, not a gift: candidate
    Item b = { 6, 101, true, 1, false };   // ordinary gift: skipped
    Item c = { 7, 130, false, 0, false };  // more than a quarter over: rejected
    Item d = { 8, 99, true, 2, true };     // wedding ring: skipped
    shop.stock.push_back(a); shop.stock.push_back(b); shop.stock.push_back(c); shop.stock.push_back(d);
    Building dead = shop; dead.live = false; Item e = { 9, 100, false, 0, false }; dead.stock.push_back(e);
    Building noFlag = shop; noFlag.property = 0x20; noFlag.stock.clear(); noFlag.stock.push_back(e);
    v.push_back(dead); v.push_back(noFlag); v.push_back(shop);
    Wife w = { 100, 0 };
    FindAppropriateGift(w, v, 0.0f, true);
    if (w.GiftToGiveDef != 5 || w.GiftGivingPriceValue != 10) { std::printf("006cf530_TEST FAIL\n"); return 1; }
    Wife w2 = { 200, 0 };   // budget 200/2 = 100 with r = 1
    FindAppropriateGift(w2, v, 1.0f, false);
    if (w2.GiftToGiveDef != 5 || w2.GiftGivingPriceValue != 110) { std::printf("006cf530_TEST FAIL\n"); return 1; }
    Wife w3 = { 0, 3 };     // nothing qualifies: unchanged
    FindAppropriateGift(w3, v, 0.0f, true);
    if (w3.GiftToGiveDef != 3 || w3.GiftGivingPriceValue != 0) { std::printf("006cf530_TEST FAIL\n"); return 1; }
    std::printf("006cf530_TEST PASS\n");
    return 0;
}