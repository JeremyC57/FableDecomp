// Behaviour test for 0x005AECC0 CTCInventoryTrade::SellItem: selling `count` of an item lowers its sell entry, its branch's and its
// category's sellable totals, deletes the entry (and the branch arrow when the branch empties) at zero, pays the hero the shop's buy
// price, records each sale with the shop, removes the items from the hero's inventory and moves them onto the buy side: an existing
// buy entry for the same definition gains `count` (re-enabled with an arrow when it was at zero), else the item is added to the
// branch's buy list (a new sub-list when the branch has none).
#include <cstdio>
#include <map>

struct Trade {
    std::map<int, std::pair<long, long> > to_sell;          // item -> (def, count)
    std::map<int, std::pair<long, long> > to_buy;           // item -> (def, count)
    std::map<long, long> branch_sellable;                   // def -> branch count
    std::map<long, int> buy_lists;                          // branch -> list
    long total_items, money, sold, inventory, arrows, sub_lists, added_to_list, deleted;
    Trade() : total_items(0), money(0), sold(0), inventory(0), arrows(0), sub_lists(0), added_to_list(0), deleted(0) { }
    long BuyPrice(long, long n) { return 7 * n; }
    void SellItem(int item, long count)
    {
        std::pair<long, long>& entry = to_sell[item];
        long def = entry.first;
        entry.second -= count;
        branch_sellable[def] -= count;
        total_items -= count;
        if (entry.second == 0) {
            ++deleted;
            to_sell.erase(item);
            if (branch_sellable[def] <= 0) ++arrows;
        }
        long price = BuyPrice(def, count);
        for (long i = 0; i < count; ++i) ++sold;
        money += price;
        inventory -= count;
        std::map<int, std::pair<long, long> >::iterator it;
        for (it = to_buy.begin(); it != to_buy.end(); ++it) {
            if (def == it->second.first) {
                long old = it->second.second;
                it->second.second += count;
                if (old == 0) ++arrows;
                return;
            }
        }
        if (buy_lists.find(def) != buy_lists.end()) ++added_to_list;
        else { ++sub_lists; ++added_to_list; buy_lists[def] = 1; }
    }
};

int main()
{
    Trade t;
    t.to_sell[1] = std::make_pair(10L, 3L);
    t.to_sell[2] = std::make_pair(20L, 1L);
    t.branch_sellable[10] = 3; t.branch_sellable[20] = 1;
    t.total_items = 4; t.inventory = 4;
    t.to_buy[5] = std::make_pair(10L, 0L);
    t.SellItem(1, 2);
    bool ok = t.to_sell[1].second == 1 && t.money == 14 && t.sold == 2 && t.inventory == 2 && t.to_buy[5].second == 2 && t.arrows == 1
        && t.deleted == 0 && t.total_items == 2;
    t.SellItem(2, 1);
    ok = ok && t.to_sell.find(2) == t.to_sell.end() && t.deleted == 1 && t.arrows == 2 && t.sub_lists == 1 && t.added_to_list == 1
        && t.money == 21;
    t.to_sell[3] = std::make_pair(20L, 1L);
    t.branch_sellable[20] = 1;
    t.SellItem(3, 1);
    ok = ok && t.sub_lists == 1 && t.added_to_list == 2 && t.sold == 4;
    std::printf(ok ? "005aecc0_TEST PASS\n" : "005aecc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
