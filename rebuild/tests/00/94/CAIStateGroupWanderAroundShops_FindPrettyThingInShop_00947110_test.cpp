// behaviour test for 0x00947110 CAIStateGroup_WanderAroundShops::FindPrettyThingInShop(CThing* shop)
#include <cstdio>
#include <vector>
class CThing { public: bool dead; bool hasOwner; bool pretty; std::vector<CThing*> stock; };
static unsigned long seed = 7;
static unsigned long GFRandom(unsigned long range) { seed = seed * 0x24A1 + 0x24DF; seed = (seed >> 13) | (seed << 19); return seed % range; }
static CThing* FindPrettyThingInShop(CThing* shop)
{
    if (shop && !shop->dead && shop->hasOwner) {
        std::vector<CThing*> pretty;
        for (size_t i = 0; i < shop->stock.size(); ++i) {
            CThing* t = shop->stock[i];
            if (t && !t->dead && t->pretty) pretty.push_back(t);
        }
        if (!pretty.empty()) return pretty[GFRandom(pretty.size())];
    }
    return 0;
}
int main()
{
    CThing a = { false, false, true }, b = { true, false, true }, c = { false, false, false };
    CThing shop = { false, true, false };
    shop.stock.push_back(&a); shop.stock.push_back(&b); shop.stock.push_back(&c); shop.stock.push_back(0);
    bool ok = FindPrettyThingInShop(&shop) == &a && FindPrettyThingInShop(0) == 0;
    CThing deadShop = shop; deadShop.dead = true;
    ok = ok && FindPrettyThingInShop(&deadShop) == 0;
    CThing empty = { false, true, false };
    ok = ok && FindPrettyThingInShop(&empty) == 0;
    std::printf(ok ? "00947110_TEST PASS\n" : "00947110_TEST FAIL\n");
    return ok ? 0 : 1;
}