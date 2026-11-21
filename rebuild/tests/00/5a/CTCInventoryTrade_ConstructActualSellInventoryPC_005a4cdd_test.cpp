// Behaviour test for CTCInventoryTrade::ConstructActualSellInventoryPC @ 0x005A4CDD: the PC "sell" trade screen. Models what the
// function decides: the hero's sellable items are collected only for an ordinary shop (none for a barber, tattooist or title vendor);
// tattoo and hair cards are dropped from both the sellable list and the shop's buyable list; ItemsToBuy maps a running index
// (0, 1, 2 ...) to each remaining buyable pair; one sub list per menu branch sized by the number of sellable entries in it; for every
// entry AddSellItemToListPC accepted, its count is added to the branch's sell total and to the clothing / weapons / items total of its
// menu type; the branch label gets " x<n>" only when its total is positive, the three category labels always get " x<total>"; the
// "sell" total is items + clothes + weapons; the subtitle text and static name follow the entry's name; the closing refresh runs only
// for an ordinary shop.
#include <cstdio>
#include <map>
#include <string>
#include <vector>

enum EItem_Menu_Type { MENU_TYPE_CLOTHING, MENU_TYPE_WEAPONS, MENU_TYPE_ITEMS, ITEM_MENU_TYPE_NONE };
struct ItemInfo { int branch; EItem_Menu_Type type; bool tattooCard; bool hairCard; bool addable; };
typedef std::pair<long, long> Item;                                         // (itemDefIndex, count)
typedef std::map<long, ItemInfo> Defs;

static std::string IntToString(long value)                                  // GFIntToWideString
{
    char buf[32];
    std::sprintf(buf, "%ld", value);
    return buf;
}

static bool OrdinaryShop(bool barber, bool tattoo, bool title) { return !barber && !tattoo && !title; }

static void DropCards(std::vector<Item>& items, const Defs& defs)           // the two card-filter loops
{
    for (std::vector<Item>::iterator it = items.begin(); it != items.end(); ) {
        const ItemInfo& info = defs.find(it->first)->second;
        if (info.tattooCard || info.hairCard)
            it = items.erase(it);
        else
            ++it;
    }
}

struct SellScreen {
    std::map<long, Item> itemsToBuy;                                        // ItemsToBuy, keyed by the running index
    std::map<int, long> subListSize;                                        // InitialiseOffsets(count) per branch
    std::map<int, long> branchTotal;                                        // NumSellableThings[branch].second
    long clothes, weapons, items;                                           // TotNumClothesToSell / Weapons / Items
};

static SellScreen Build(bool barber, bool tattoo, bool title, const std::vector<Item>& heroItems, std::vector<Item> buyable,
                        const Defs& defs)
{
    SellScreen s;
    s.clothes = s.weapons = s.items = 0;
    std::vector<Item> all;
    if (OrdinaryShop(barber, tattoo, title))
        all = heroItems;
    DropCards(all, defs);
    DropCards(buyable, defs);
    long index = 0;
    for (std::vector<Item>::const_iterator it = buyable.begin(); it != buyable.end(); ++it)
        s.itemsToBuy[index++] = *it;
    if (all.begin() != all.end()) {
        for (std::vector<Item>::const_iterator it = all.begin(); it != all.end(); ++it) {
            int branch = defs.find(it->first)->second.branch;
            std::map<int, long>::iterator found = s.subListSize.find(branch);
            if (found == s.subListSize.end()) s.subListSize[branch] = 1; else ++found->second;
        }
        for (std::vector<Item>::const_iterator it = all.begin(); it != all.end(); ++it) {
            const ItemInfo& info = defs.find(it->first)->second;
            if (!info.addable)                                              // AddSellItemToListPC returned false
                continue;
            if (s.branchTotal.find(info.branch) == s.branchTotal.end()) s.branchTotal[info.branch] = it->second;
            else s.branchTotal[info.branch] += it->second;
            switch (info.type) {
            case MENU_TYPE_CLOTHING: s.clothes += it->second; break;
            case MENU_TYPE_WEAPONS: s.weapons += it->second; break;
            case MENU_TYPE_ITEMS:   s.items += it->second;   break;
            default: break;
            }
        }
    }
    return s;
}

static std::string BranchLabel(const std::string& label, long total, const std::string& x)
{
    std::string text = label;
    if (total > 0) text += x + IntToString(total);
    return text;
}

static std::string CategoryLabel(const std::string& label, long total, const std::string& x) { return label + x + IntToString(total); }

static std::string Subtitle(const std::string& entryName, const SellScreen& s, const std::string& x, std::string* staticName)
{
    *staticName = "";
    if (entryName == "PC_TRADING_MENU_CLOTHING_ENTRY") { *staticName = "Clothes_Subtitle"; return "TEXT_GUI_MENU_CLOTHING" + x + IntToString(s.clothes); }
    if (entryName == "PC_TRADING_MENU_WEAPONS_ENTRY") { *staticName = "Weapons_Subtitle"; return "TEXT_GUI_MENU_WEAPONS" + x + IntToString(s.weapons); }
    if (entryName == "PC_TRADING_MENU_ITEMS_ENTRY") { *staticName = "Items_Subtitle"; return "TEXT_GUI_MENU_ITEMS" + x + IntToString(s.items); }
    return "";
}

int main()
{
    int ok = 1;
    Defs defs;
    ItemInfo shirt = { 10, MENU_TYPE_CLOTHING, false, false, true }, sword = { 20, MENU_TYPE_WEAPONS, false, false, true };
    ItemInfo potion = { 30, MENU_TYPE_ITEMS, false, false, true }, tattoo = { 30, MENU_TYPE_ITEMS, true, false, true };
    ItemInfo hair = { 30, MENU_TYPE_ITEMS, false, true, true }, quest = { 30, MENU_TYPE_ITEMS, false, false, false };
    defs[1] = shirt; defs[2] = sword; defs[3] = potion; defs[4] = tattoo; defs[5] = hair; defs[6] = quest;
    std::vector<Item> hero;
    hero.push_back(Item(1, 2)); hero.push_back(Item(4, 1)); hero.push_back(Item(2, 1)); hero.push_back(Item(5, 3));
    hero.push_back(Item(3, 5)); hero.push_back(Item(6, 7)); hero.push_back(Item(1, 1));
    std::vector<Item> buyable;
    buyable.push_back(Item(5, 1)); buyable.push_back(Item(2, 4)); buyable.push_back(Item(4, 2)); buyable.push_back(Item(3, 9));

    SellScreen s = Build(false, false, false, hero, buyable, defs);
    if (s.itemsToBuy.size() != 2 || s.itemsToBuy[0] != Item(2, 4) || s.itemsToBuy[1] != Item(3, 9)) ok = 0;   // cards dropped, index 0..
    if (s.subListSize.size() != 3 || s.subListSize[10] != 2 || s.subListSize[20] != 1 || s.subListSize[30] != 2) ok = 0;
    if (s.branchTotal[10] != 3 || s.branchTotal[20] != 1 || s.branchTotal[30] != 5) ok = 0;                    // quest item not added
    if (s.clothes != 3 || s.weapons != 1 || s.items != 5) ok = 0;
    if (s.items + s.clothes + s.weapons != 9) ok = 0;                                                         // "TEXT_GUI_MENU_SELL x9"

    if (BranchLabel("Shirts", 3, " x") != "Shirts x3") ok = 0;
    if (BranchLabel("Shirts", 0, " x") != "Shirts") ok = 0;
    if (CategoryLabel("Weapons", 0, " x") != "Weapons x0") ok = 0;

    std::string name;
    if (Subtitle("PC_TRADING_MENU_CLOTHING_ENTRY", s, " x", &name) != "TEXT_GUI_MENU_CLOTHING x3" || name != "Clothes_Subtitle") ok = 0;
    if (Subtitle("PC_TRADING_MENU_WEAPONS_ENTRY", s, " x", &name) != "TEXT_GUI_MENU_WEAPONS x1" || name != "Weapons_Subtitle") ok = 0;
    if (Subtitle("PC_TRADING_MENU_ITEMS_ENTRY", s, " x", &name) != "TEXT_GUI_MENU_ITEMS x5" || name != "Items_Subtitle") ok = 0;
    if (Subtitle("PC_TRADING_MENU_OTHER_ENTRY", s, " x", &name) != "" || name != "") ok = 0;

    SellScreen b = Build(true, false, false, hero, buyable, defs);           // barber: nothing to sell, buy list still indexed
    if (!b.subListSize.empty() || b.clothes != 0 || b.weapons != 0 || b.items != 0 || b.itemsToBuy.size() != 2) ok = 0;
    SellScreen t = Build(false, true, false, hero, buyable, defs);
    SellScreen h = Build(false, false, true, hero, buyable, defs);
    if (!t.subListSize.empty() || !h.subListSize.empty()) ok = 0;

    if (!OrdinaryShop(false, false, false)) ok = 0;                          // closing refresh only for an ordinary shop
    if (OrdinaryShop(true, false, false) || OrdinaryShop(false, true, false) || OrdinaryShop(false, false, true)) ok = 0;

    if (!ok) { std::printf("005a4cdd_TEST FAIL\n"); return 1; }
    std::printf("005a4cdd_TEST PASS\n");
    return 0;
}