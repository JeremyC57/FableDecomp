// Behaviour test for 0x0059DE76 CTCInventoryTrade::UpdateSellActionsList (model copy: ability / barber / tattoo shops leave the sell lists alone; otherwise every value component receives a type-3 action naming its key and the map is cleared)
#include <cstdio>
struct Component;
struct Action {
    int type; Component* params[4]; int nparams;
    Action() : type(0), nparams(0) {}
    void push_back(Component* c) { params[nparams++] = c; }
};
struct Component {
    int got; Action last;
    Component() : got(0) {}
    void SetAction(Action a) { last = a; ++got; }
};
struct Entry { Component* first; Component* second; };
struct FakeMap {
    Entry e[4]; int n;
    int begin() const { return 0; }
    int end() const { return n; }
    void clear() { n = 0; }
};
struct Shop {
    int kind;   // 0 general, 1 ability, 2 barber, 3 tattoo
    bool IsAbilityShop() const { return kind == 1; }
    bool IsBarberShop() const { return kind == 2; }
    bool IsTattooShop() const { return kind == 3; }
};
struct Trade {
    FakeMap ItemSellLists; Shop* shop;
    void UpdateSellActionsList()
    {
        if (shop->IsAbilityShop() || shop->IsBarberShop() || shop->IsTattooShop())
            return;
        for (int it = ItemSellLists.begin(); it != ItemSellLists.end(); ++it) {
            Action action;
            action.type = 3;
            action.push_back(ItemSellLists.e[it].first);
            ItemSellLists.e[it].second->SetAction(action);
        }
        ItemSellLists.clear();
    }
};
int main()
{
    for (int kind = 1; kind <= 3; ++kind) {
        Component k, v; Shop s; s.kind = kind;
        Trade t; t.shop = &s; t.ItemSellLists.n = 1; t.ItemSellLists.e[0].first = &k; t.ItemSellLists.e[0].second = &v;
        t.UpdateSellActionsList();
        if (v.got != 0 || t.ItemSellLists.n != 1) { std::printf("0059de76_TEST FAIL\n"); return 1; }
    }
    Component k1, v1, k2, v2; Shop s; s.kind = 0;
    Trade t; t.shop = &s; t.ItemSellLists.n = 2;
    t.ItemSellLists.e[0].first = &k1; t.ItemSellLists.e[0].second = &v1;
    t.ItemSellLists.e[1].first = &k2; t.ItemSellLists.e[1].second = &v2;
    t.UpdateSellActionsList();
    if (v1.got != 1 || v1.last.type != 3 || v1.last.params[0] != &k1 || v2.got != 1 || v2.last.params[0] != &k2 || t.ItemSellLists.n != 0) { std::printf("0059de76_TEST FAIL\n"); return 1; }
    std::printf("0059de76_TEST PASS\n");
    return 0;
}