// Behaviour test for CTCInventoryTrade::RefreshBuyPossibility @ 0x005A9A87: an entry is greyed out (and its selection arrow and
// buy-actions list scheduled for deletion) when the item is too dear, or has no room and is not a special case, or is already
// owned; the buy counts are refreshed once at the end.
#include <cstdio>
#include <vector>
struct Item { long def, count, price; bool room, special, owned; bool greyed; int arrowDeletion; int actionsDeletion; int parents; };
static int gNumbersRefreshed;
static void RefreshBuyPossibility(std::vector<Item>& items, long money)
{
    for (unsigned int i = 0; i < items.size(); ++i) {
        Item& it = items[i];
        if (money < it.price || (it.count > 0 && !it.room && !it.special) || it.owned) {
            it.greyed = true;
            it.arrowDeletion = 1;          // UI_DELETE_THIS_INSTANCE
            it.actionsDeletion = 3;        // UI_DELETE_THIS_INSTANCE_PER_PARENT
            it.parents = 1;                // the entry itself
        }
    }
    ++gNumbersRefreshed;
}
int main()
{
    int ok = 1;
    Item a = { 1, 1, 50, true, false, false, false, 0, 0, 0 };    // affordable, room: stays
    Item b = { 2, 1, 500, true, false, false, false, 0, 0, 0 };   // too dear
    Item c = { 3, 2, 10, false, false, false, false, 0, 0, 0 };   // no room, not special
    Item d = { 4, 2, 10, false, true, false, false, 0, 0, 0 };    // no room but special: stays
    Item e = { 5, 0, 10, false, false, false, false, 0, 0, 0 };   // count 0: the room test is skipped
    Item f = { 6, 1, 10, true, false, true, false, 0, 0, 0 };     // already owned
    std::vector<Item> v; v.push_back(a); v.push_back(b); v.push_back(c); v.push_back(d); v.push_back(e); v.push_back(f);
    RefreshBuyPossibility(v, 100);
    bool want[6] = { false, true, true, false, false, true };
    for (int i = 0; i < 6; ++i) {
        if (v[i].greyed != want[i]) ok = 0;
        if (want[i] && (v[i].arrowDeletion != 1 || v[i].actionsDeletion != 3 || v[i].parents != 1)) ok = 0;
    }
    if (gNumbersRefreshed != 1) ok = 0;
    if (!ok) { std::printf("005a9a87_TEST FAIL\n"); return 1; }
    std::printf("005a9a87_TEST PASS\n");
    return 0;
}