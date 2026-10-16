#include <cstdio>
// Behaviour model of CTCStockItem::GetShopTC (0x007F4FC0): through the owner's link component to the linked thing, then that
// thing's shop component, else null at every missing step.
struct Shop { int id; };
struct Linked { Shop* shop; };
struct Owner { bool hasLink; Linked* linked; };
static Shop* GetShop(const Owner& o)
{
    if (o.hasLink) {
        if (o.linked) {
            Shop* shop = 0;
            if (o.linked->shop) shop = o.linked->shop;
            return shop;
        }
    }
    return 0;
}
int main()
{
    Shop s = { 1 }; Linked l = { &s }, e = { 0 };
    Owner a = { true, &l }, b = { true, &e }, c = { true, 0 }, d = { false, &l };
    if (GetShop(a) != &s || GetShop(b) || GetShop(c) || GetShop(d)) { std::printf("007f4fc0_TEST FAIL\n"); return 1; }
    std::printf("007f4fc0_TEST PASS\n");
    return 0;
}