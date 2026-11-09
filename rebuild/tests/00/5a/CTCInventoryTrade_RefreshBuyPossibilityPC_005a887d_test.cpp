// Behaviour test for CTCInventoryTrade::RefreshBuyPossibilityPC 0x005A887D: an entry is greyed out (MakeUnavailableDynamically)
// when the hero's money is below the shop's price for one, or (stocked and) the hero's category inventory has no room and it is
// not a special case, or the hero already has the special; a greyed entry with a "PC_LIST_BUY_ACTIONS" child gets a deletion
// (method 3, parent = the entry); the buy numbers are refreshed once at the end. The inventory is only looked up for a stocked
// entry, and the room / special-case tests short-circuit as the || / && chain.
#include <cstdio>
#include <cstring>
#include <map>
#include <list>
#include <utility>

struct CComponent;
struct CDeletion { int Method; std::list<CComponent*> AssociatedParents; };
struct CComponent {
    bool Unavailable; bool HasActions; CComponent* Actions; int DeletionMethod; CComponent* DeletionParent; int DeletionCount;
    void MakeUnavailableDynamically() { Unavailable = true; }
    CComponent* FindChild(const char* name) { return (HasActions && std::strcmp(name, "PC_LIST_BUY_ACTIONS") == 0) ? Actions : 0; }
    void SetDeletion(CDeletion d)
    {
        ++DeletionCount; DeletionMethod = d.Method;
        DeletionParent = d.AssociatedParents.size() == 1 ? d.AssociatedParents.front() : 0;
    }
};
struct CInventory { long Room; int Queries; bool IsSpaceInInventoryForItem(long def) { ++Queries; return def < Room; } };
static int gInventoryLookups = 0;
struct CTrade {
    std::map<CComponent*, std::pair<long, long> > ItemsToBuy;
    long Money; long Price; CInventory Inventory; long SpecialDef; long OwnedSpecialDef; int NumbersRefreshed;
    long GetCurrentSellPrice(long def, long count) { return count == 1 ? Price + def : -1; }
    bool IsSpecialCase(long def) { return def == SpecialDef; }
    bool AlreadyHaveSpecial(long def) { return def == OwnedSpecialDef; }
    void RefreshBuyPossibilityNumbersPC() { ++NumbersRefreshed; }
    void RefreshBuyPossibilityPC()
    {
        for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
            CComponent* entry = it->first;
            CInventory* inventory = 0;
            if (it->second.second > 0) { inventory = &Inventory; ++gInventoryLookups; }
            long money = Money;
            if (money < GetCurrentSellPrice(it->second.first, 1) ||
                (it->second.second > 0 && !inventory->IsSpaceInInventoryForItem(it->second.first) && !IsSpecialCase(it->second.first)) ||
                AlreadyHaveSpecial(it->second.first)) {
                entry->MakeUnavailableDynamically();
                CComponent* actions = entry->FindChild("PC_LIST_BUY_ACTIONS");
                if (actions) {
                    CDeletion deletion;
                    deletion.Method = 3;
                    deletion.AssociatedParents.push_back(entry);
                    actions->SetDeletion(deletion);
                }
            }
        }
        RefreshBuyPossibilityNumbersPC();
    }
};

int main()
{
    bool ok = true;
    CComponent actions[6]; std::memset(actions, 0, sizeof(actions));
    CComponent entries[6]; std::memset(entries, 0, sizeof(entries));
    for (int i = 0; i < 6; ++i) { entries[i].HasActions = true; entries[i].Actions = &actions[i]; }
    entries[5].HasActions = false;
    CTrade trade;
    trade.Money = 100; trade.Price = 90; trade.Inventory.Room = 8; trade.Inventory.Queries = 0;
    trade.SpecialDef = 9; trade.OwnedSpecialDef = 7; trade.NumbersRefreshed = 0;
    trade.ItemsToBuy[&entries[0]] = std::make_pair(5L, 2L);     // affordable, room: stays
    trade.ItemsToBuy[&entries[1]] = std::make_pair(20L, 1L);    // too dear (110)
    trade.ItemsToBuy[&entries[2]] = std::make_pair(8L, 1L);     // no room, not special
    trade.ItemsToBuy[&entries[3]] = std::make_pair(9L, 1L);     // no room but special: stays
    trade.ItemsToBuy[&entries[4]] = std::make_pair(7L, 0L);     // already owned special (not stocked: no inventory lookup)
    trade.ItemsToBuy[&entries[5]] = std::make_pair(30L, 0L);    // too dear, no actions child
    trade.RefreshBuyPossibilityPC();
    const bool greyed[6] = { false, true, true, false, true, true };
    for (int i = 0; i < 6; ++i) {
        if (entries[i].Unavailable != greyed[i]) ok = false;
        int expected = (greyed[i] && entries[i].HasActions) ? 1 : 0;
        if (actions[i].DeletionCount != expected) ok = false;
        if (expected && (actions[i].DeletionMethod != 3 || actions[i].DeletionParent != &entries[i])) ok = false;
    }
    if (gInventoryLookups != 4 || trade.Inventory.Queries != 3 || trade.NumbersRefreshed != 1) ok = false;
    if (!ok) { std::printf("005a887d_TEST FAIL\n"); return 1; }
    std::printf("005a887d_TEST PASS\n");
    return 0;
}