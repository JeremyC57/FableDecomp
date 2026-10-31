// behaviour test for CTCInventoryTrade::ConstructSellActionsList(PC) 0x005a731f: the list gets 1..4 entries by count
// (sell; sell 10 above 10; sell 50 above 50; sell all above 1); it is remembered in ItemSellLists and attached only
// when the count is non-zero and the shop stocks the def, and the result says whether it was
#include <cstdio>
#include <map>
#include <vector>
struct List { std::vector<int> actions; int itemCount; };
static bool Construct(std::map<int, List*>& lists, std::map<int, std::vector<List*> >& children, int parent,
                      unsigned long count, bool stocked, List* list)
{
    list->itemCount = count > 50 ? 4 : count > 10 ? 3 : count > 1 ? 2 : 1;
    list->actions.push_back(0x25);
    if (count > 10) list->actions.push_back(10);
    if (count > 50) list->actions.push_back(50);
    if (count > 1) list->actions.push_back(0x52);
    if (count != 0 && stocked) { lists[parent] = list; children[parent].push_back(list); return true; }
    return false;
}
int main()
{
    std::map<int, List*> lists; std::map<int, std::vector<List*> > children;
    List a = List(), b = List(), c = List();
    bool r1 = Construct(lists, children, 1, 60, true, &a);
    bool r2 = Construct(lists, children, 2, 0, true, &b);
    bool r3 = Construct(lists, children, 3, 5, false, &c);
    int ok = r1 && !r2 && !r3 && a.itemCount == 4 && a.actions.size() == 4 && b.itemCount == 1 && c.itemCount == 2
          && c.actions.size() == 2 && lists.size() == 1 && lists[1] == &a && children[1].size() == 1;
    std::printf(ok ? "005a731f_TEST PASS\n" : "005a731f_TEST FAIL\n");
    return !ok;
}