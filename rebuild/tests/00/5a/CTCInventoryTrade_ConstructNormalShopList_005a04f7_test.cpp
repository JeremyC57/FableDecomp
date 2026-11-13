// Behaviour test for CTCInventoryTrade::ConstructNormalShopList 0x005A04F7: a copy of the row's per-section logic over a small
// model (menu branches by def index, Add*ItemToList acceptance, the per-branch / per-category counts, the entry captions
// "<text> x <total>", the NotInStock entries added with count 0 and the shop-name screen title fallback).
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <utility>

enum EItem_Menu_Type { ITEM_MENU_TYPE_CLOTHES = 0, ITEM_MENU_TYPE_WEAPONS = 1, ITEM_MENU_TYPE_ITEMS = 2 };
typedef int Branch;                                   // a menu branch component (model: an id)

static Branch GetMenuBranchFromDefIndex(long def, EItem_Menu_Type& type)
{
    if (def >= 100 && def < 200) { type = ITEM_MENU_TYPE_WEAPONS; return def < 150 ? 1 : 2; }   // melee / ranged
    if (def >= 200 && def < 300) { type = ITEM_MENU_TYPE_CLOTHES; return 3; }
    type = ITEM_MENU_TYPE_ITEMS; return 4;
}
static std::string IntToString(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

struct Trade {
    std::map<Branch, long> subListSizes;              // XListsByCategory + InitialiseOffsets
    std::map<Branch, long> num;                       // NumXThings[branch].second
    long totClothes, totWeapons, totItems;
    std::vector<std::pair<long, long> > added;        // (def, count) given to Add*ItemToList
    std::string caption;
};
static bool Accept(long def) { return def % 7 != 0; }  // Add{Sell,Buy}ItemToList's own refusal (model)

static void SellOrBuy(Trade& t, const std::vector<std::pair<long, long> >& items, const std::list<long>* notInStock, const char* text)
{
    t.totClothes = t.totWeapons = t.totItems = 0;
    if (!items.empty() || (notInStock && !notInStock->empty())) {
        std::map<Branch, long> branchCounts;
        for (std::vector<std::pair<long, long> >::const_iterator it = items.begin(); it != items.end(); ++it) {
            EItem_Menu_Type type; Branch b = GetMenuBranchFromDefIndex(it->first, type);
            std::map<Branch, long>::iterator f = branchCounts.find(b);
            if (f == branchCounts.end()) branchCounts[b] = 1; else ++f->second;
        }
        if (notInStock)
            for (std::list<long>::const_iterator n = notInStock->begin(); n != notInStock->end(); ++n) {
                EItem_Menu_Type type; Branch b = GetMenuBranchFromDefIndex(*n, type);
                std::map<Branch, long>::iterator f = branchCounts.find(b);
                if (f == branchCounts.end()) branchCounts[b] = 1; else ++f->second;
            }
        t.subListSizes = branchCounts;
        t.num.clear();
        for (std::vector<std::pair<long, long> >::const_iterator it = items.begin(); it != items.end(); ++it) {
            EItem_Menu_Type type; Branch b = GetMenuBranchFromDefIndex(it->first, type);
            t.added.push_back(*it);
            if (Accept(it->first)) {
                if (t.num.find(b) == t.num.end()) t.num[b] = it->second; else t.num[b] += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: t.totClothes += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: t.totWeapons += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   t.totItems   += it->second; break;
                }
            }
        }
        if (notInStock)
            for (std::list<long>::const_iterator n = notInStock->begin(); n != notInStock->end(); ++n)
                t.added.push_back(std::make_pair(*n, 0L));
    }
    t.caption = std::string(text) + " x " + IntToString(t.totWeapons + t.totItems + t.totClothes);
}

static void Wanted(Trade& t, const std::vector<long>& items)
{
    t.totClothes = t.totWeapons = t.totItems = 0;
    if (items.begin() != items.end()) {
        std::map<Branch, long> branchCounts;
        for (std::vector<long>::const_iterator it = items.begin(); it != items.end(); ++it) {
            EItem_Menu_Type type; Branch b = GetMenuBranchFromDefIndex(*it, type);
            std::map<Branch, long>::iterator f = branchCounts.find(b);
            if (f == branchCounts.end()) branchCounts[b] = 1; else ++f->second;
        }
        t.subListSizes = branchCounts;
        t.num.clear();
        for (std::vector<long>::const_iterator it = items.begin(); it != items.end(); ++it) {
            EItem_Menu_Type type; Branch b = GetMenuBranchFromDefIndex(*it, type);
            t.added.push_back(std::make_pair(*it, 1L));
            if (t.num.find(b) == t.num.end()) t.num[b] = 1; else ++t.num[b];
            switch (type) {
            case ITEM_MENU_TYPE_CLOTHES: ++t.totClothes; break;
            case ITEM_MENU_TYPE_WEAPONS: ++t.totWeapons; break;
            case ITEM_MENU_TYPE_ITEMS:   ++t.totItems;   break;
            }
        }
    }
    t.caption = std::string("Wanted x ") + IntToString(t.totWeapons + t.totItems + t.totClothes);
}

static std::string ScreenTitle(unsigned long shopName, bool inBank) { return (shopName > 0 && inBank) ? "ShopName" : "Trade"; }

int main()
{
    int bad = 0;
    std::vector<std::pair<long, long> > sell;
    sell.push_back(std::make_pair(101L, 2L)); sell.push_back(std::make_pair(160L, 1L));
    sell.push_back(std::make_pair(211L, 3L)); sell.push_back(std::make_pair(14L, 5L));   // 14: refused by AddSellItemToList
    sell.push_back(std::make_pair(302L, 4L));
    Trade s; SellOrBuy(s, sell, 0, "Sell");
    if (s.subListSizes.size() != 4 || s.subListSizes[4] != 2 || s.subListSizes[1] != 1) bad |= 1;
    if (s.totWeapons != 3 || s.totClothes != 3 || s.totItems != 4) bad |= 2;
    if (s.num[4] != 4 || s.num.find(1) == s.num.end() || s.caption != "Sell x 10") bad |= 4;

    std::vector<std::pair<long, long> > buy; buy.push_back(std::make_pair(120L, 1L));
    std::list<long> notInStock; notInStock.push_back(220L); notInStock.push_back(130L);
    Trade b; SellOrBuy(b, buy, &notInStock, "Buy");
    if (b.subListSizes[1] != 2 || b.subListSizes[3] != 1 || b.totWeapons != 1 || b.caption != "Buy x 1") bad |= 8;
    if (b.added.size() != 3 || b.added[1].second != 0 || b.added[2].first != 130) bad |= 16;
    Trade e; std::vector<std::pair<long, long> > none; SellOrBuy(e, none, 0, "Buy");
    if (!e.subListSizes.empty() || e.caption != "Buy x 0") bad |= 32;

    std::vector<long> wanted; wanted.push_back(250L); wanted.push_back(260L); wanted.push_back(5L);
    Trade w; Wanted(w, wanted);
    if (w.totClothes != 2 || w.totItems != 1 || w.num[3] != 2 || w.caption != "Wanted x 3") bad |= 64;
    if (ScreenTitle(0, true) != "Trade" || ScreenTitle(7, false) != "Trade" || ScreenTitle(7, true) != "ShopName") bad |= 128;

    if (bad) { std::printf("005a04f7_TEST FAIL\n"); return 1; }
    std::printf("005a04f7_TEST PASS\n");
    return 0;
}