// Behaviour test for 0x0059DE04 CTCInventoryTrade::UpdateBuyActionsList (model copy: every value component receives a type-3 action whose parameter list holds its key, then the map is cleared)
#include <cstdio>
struct Component;
struct Action {
    int type; Component* params[4]; int nparams;
    Action() : type(0), nparams(0) {}
    void push_back(Component* c) { params[nparams++] = c; }
};
struct Component {
    int id; int got; Action last;
    Component(int i) : id(i), got(0) {}
    void SetAction(Action a) { last = a; ++got; }
};
struct Entry { Component* first; Component* second; };
struct FakeMap {
    Entry e[4]; int n;
    int begin() const { return 0; }
    int end() const { return n; }
    void clear() { n = 0; }
};
struct Trade {
    FakeMap ItemBuyLists;
    void UpdateBuyActionsList()
    {
        for (int it = ItemBuyLists.begin(); it != ItemBuyLists.end(); ++it) {
            Action action;
            action.type = 3;
            action.push_back(ItemBuyLists.e[it].first);
            ItemBuyLists.e[it].second->SetAction(action);
        }
        ItemBuyLists.clear();
    }
};
int main()
{
    Component k1(1), v1(2), k2(3), v2(4);
    Trade t; t.ItemBuyLists.n = 2;
    t.ItemBuyLists.e[0].first = &k1; t.ItemBuyLists.e[0].second = &v1;
    t.ItemBuyLists.e[1].first = &k2; t.ItemBuyLists.e[1].second = &v2;
    t.UpdateBuyActionsList();
    if (v1.got != 1 || v1.last.type != 3 || v1.last.nparams != 1 || v1.last.params[0] != &k1) { std::printf("0059de04_TEST FAIL\n"); return 1; }
    if (v2.got != 1 || v2.last.type != 3 || v2.last.nparams != 1 || v2.last.params[0] != &k2) { std::printf("0059de04_TEST FAIL\n"); return 1; }
    if (k1.got != 0 || t.ItemBuyLists.n != 0) { std::printf("0059de04_TEST FAIL\n"); return 1; }
    t.UpdateBuyActionsList();
    if (v1.got != 1) { std::printf("0059de04_TEST FAIL\n"); return 1; }
    std::printf("0059de04_TEST PASS\n");
    return 0;
}