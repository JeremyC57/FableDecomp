// Behaviour test for CTCInventoryTrade::ConstructBuyActionsListPC @ 0x005A79E6 (PC twin of the console 0x005A3E47; differs only in the
// list / entry template names): the buyable quantity is the stock count capped by what the hero's inventory can hold, then stepped
// down until affordable; the list gets 4/3/2/1 offset slots for quantity > 50 / > 10 / > 1 / else, a "Buy" entry (UI_ACTION_TYPE_BUY),
// then "Buy 10", "Buy 50" and "Buy all (N)" bulk entries (UI_ACTION_TYPE_BUY_BULK with the amount) when the quantity exceeds 10 / 50
// / 1 and the shop is not a barber, tattooist or title shop. The list is attached (and recorded for the parent) only when one item is
// affordable, there is room or it is a special, and the stock count is non-zero.
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#define PC_TWIN
#ifndef PC_TWIN
#define ROW "005a3e47"
static const char* kListName = "UI_LIST_BUY_ACTIONS";
static const char* kTemplate = "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW";
#else
#define ROW "005a79e6"
static const char* kListName = "PC_LIST_BUY_ACTIONS";
static const char* kTemplate = "PC_MENU_ENTRY_TEMPLATE";
#endif
enum { BUY = 36, BUY_BULK = 81 };
struct Entry { std::string label, templ; long action; long amount; };
struct List { std::string name; long offsets; std::vector<Entry> entries; };
struct Shop { bool barber, tattoo, title; };
struct World { long price, money, containable; bool room, special; };
static bool Affordable(const World& w, long n) { return w.price * n <= w.money; }
static std::string Num(long n) { std::ostringstream o; o << n; return o.str(); }
static bool Construct(std::map<int, List>& lists, int parent, long count, const World& w, const Shop& shop, List& out)
{
    out.name = kListName; out.entries.clear();
    long quantity = count;
    if (w.containable <= count) quantity = w.containable;
    for (; quantity >= 1; --quantity)
        if (Affordable(w, quantity)) break;
    out.offsets = quantity > 50 ? 4 : quantity > 10 ? 3 : quantity > 1 ? 2 : 1;
    Entry buy = { "Buy", kTemplate, BUY, 0 };
    out.entries.push_back(buy);
    bool bulk = !shop.barber && !shop.tattoo && !shop.title;
    if (quantity > 10 && bulk) { Entry e = { "Buy 10", kTemplate, BUY_BULK, 10 }; out.entries.push_back(e); }
    if (quantity > 50 && bulk) { Entry e = { "Buy 50", kTemplate, BUY_BULK, 50 }; out.entries.push_back(e); }
    if (quantity > 1 && bulk) { Entry e = { "Buy all (" + Num(quantity) + ")", kTemplate, BUY_BULK, quantity }; out.entries.push_back(e); }
    if (Affordable(w, 1) && (w.room || w.special) && count != 0) { lists[parent] = out; return true; }
    return false;
}
int main()
{
    int ok = 1;
    std::map<int, List> lists;
    Shop normal = { false, false, false }, barber = { true, false, false };
    World w = { 10, 1000, 200, true, false };
    List l;
    if (!Construct(lists, 1, 80, w, normal, l)) ok = 0;
    if (l.name != kListName || l.offsets != 4 || l.entries.size() != 4) ok = 0;
    if (ok && (l.entries[0].action != BUY || l.entries[1].amount != 10 || l.entries[2].amount != 50 || l.entries[3].amount != 80 ||
               l.entries[3].label != "Buy all (80)" || l.entries[3].templ != kTemplate)) ok = 0;
    World poor = { 10, 125, 200, true, false };                 // 80 in stock, 12 affordable
    if (!Construct(lists, 2, 80, poor, normal, l) || l.offsets != 3 || l.entries.size() != 3 || l.entries[2].amount != 12) ok = 0;
    World cap = { 1, 1000, 5, true, false };                    // the inventory holds 5
    if (!Construct(lists, 3, 80, cap, normal, l) || l.offsets != 2 || l.entries.size() != 2 || l.entries[1].amount != 5) ok = 0;
    if (!Construct(lists, 4, 80, w, barber, l) || l.entries.size() != 1 || l.offsets != 4) ok = 0;   // no bulk buying at a barber
    World full = { 10, 1000, 0, false, false };
    if (Construct(lists, 5, 3, full, normal, l) || l.offsets != 1) ok = 0;                            // no room, not special
    World special = { 10, 1000, 0, false, true };
    if (!Construct(lists, 6, 1, special, normal, l)) ok = 0;
    if (Construct(lists, 7, 0, w, normal, l)) ok = 0;                                                  // out of stock
    World broke = { 10, 5, 200, true, false };
    if (Construct(lists, 8, 4, broke, normal, l)) ok = 0;
    if (lists.size() != 5 || lists.count(5) || lists.count(7) || lists.count(8)) ok = 0;
    if (!ok) { std::printf(ROW "_TEST FAIL\n"); return 1; }
    std::printf(ROW "_TEST PASS\n");
    return 0;
}