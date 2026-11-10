// Behaviour test for CTCInventoryTrade::ConstructSellActionsList @ 0x005A45F6 (console shop): the list gets 1..4 entries by count
// (sell; sell 10 above 10; sell 50 above 50; sell all above 1); it is remembered in ItemSellLists and attached only
// when the count is non-zero and the shop stocks the def, and the result says whether it was
#include <stdio.h>
#include <map>
#include <vector>
struct List { std::vector<int> actions; int itemCount; };
static bool Construct(std::map<int, List*>& lists, std::map<int, std::vector<List*> >& children, int parent,
                      long count, bool stocked, List* list)
{
    unsigned long quantity = count;
    list->itemCount = quantity > 50 ? 4 : quantity > 10 ? 3 : quantity > 1 ? 2 : 1;
    list->actions.push_back(0x25);
    if (quantity > 10) list->actions.push_back(10);
    if (quantity > 50) list->actions.push_back(50);
    if (quantity > 1) list->actions.push_back(0x52);
    if (quantity != 0 && stocked) { lists[parent] = list; children[parent].push_back(list); return true; }
    return false;
}
int main()
{
    std::map<int, List*> lists; std::map<int, std::vector<List*> > children;
    List a = List(), b = List(), c = List(), d = List();
    bool r1 = Construct(lists, children, 1, 60, true, &a);
    bool r2 = Construct(lists, children, 2, 0, true, &b);
    bool r3 = Construct(lists, children, 3, 5, false, &c);
    bool r4 = Construct(lists, children, 4, -1, true, &d);  // a negative count is a huge unsigned quantity
    int ok = r1 && !r2 && !r3 && a.itemCount == 4 && a.actions.size() == 4 && b.itemCount == 1 && c.itemCount == 2
          && c.actions.size() == 2 && lists[1] == &a && children[1].size() == 1
          && r4 && d.itemCount == 4 && lists.size() == 2;
    printf(ok ? "005a45f6_TEST PASS\n" : "005a45f6_TEST FAIL\n");
    return !ok;
}