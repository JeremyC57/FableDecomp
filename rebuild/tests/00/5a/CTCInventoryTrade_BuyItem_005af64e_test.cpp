// Behaviour test for CTCInventoryTrade::BuyItem @ 0x005AF64E: the console trade screen's purchase. Checks, against a model of the
// row's logic over mock shop / hero / UI records: the stock drop (never for a special), the selection-arrow removal rule (sold out,
// or no room for a normal item), the payment rule (cards and titles pay once, a normal item pays once after the shop sells `count`),
// the per-kind effect (tattoo applied, hair applied, title set only when the parent thing has a hero component, normal item added
// at price / count each with the weapon warning), the sell-side bookkeeping for a buyable item (per-type totals, category subtitle
// "name x N", an existing in-stock sell record grows, otherwise a new one under the branch with a new sub-list when the branch has
// none), the special-item arrow refresh (affordable, not yet owned, no buy-actions list, no arrow yet), and the refresh of the buy /
// sell entry amounts only for a normal shop.
#include <cstdio>
#include <vector>
#include <map>
#include <string>

enum Kind { NORMAL, TATTOO, HAIR, TITLE };
enum MenuType { CLOTHES = 0, WEAPONS = 1, ITEMS = 2 };

struct Item { long def; Kind kind; bool special; bool weapon; bool buyable; MenuType type; long price; };
struct Branch { std::string subtitle; long subtitleCount; bool hasSubtitleRecord; bool hasList; bool listCreated; bool available; };
struct SellRecord { long def; long count; };

struct World {
    std::map<long, Item> items;
    long money;
    bool heroHasHeroTC, weaponsUsable, hasRoom, titleShop, tattooShop, barberShop;
    // results
    long stock; bool arrowRemoved; long paid; long shopSold; bool tattooApplied, hairApplied; long titleSet; long added, eachPrice;
    bool warning; long totItems, totWeapons, totClothes; std::string sellTotal; Branch branch; std::vector<SellRecord> sellRecords;
    bool recordGrew, recordNew; int arrowsRestored; bool amountsRefreshed; bool backEvent;
};

static std::string Num(long n) { char b[32]; std::sprintf(b, "%ld", n); return b; }

// the model of the row (same control flow and arithmetic as rebuild src/005af64e.cpp)
static void BuyItem(World& w, long def, long stock, long count, std::vector<long> alreadyOwnedSpecials, std::vector<long> buyEntriesWithArrow)
{
    const Item& item = w.items[def];
    w.stock = stock;
    if (!item.special) w.stock -= count;
    w.arrowRemoved = (w.stock == 0 || (!w.hasRoom && !item.special));
    long price = item.price * count;
    w.paid = 0; w.shopSold = 0; w.tattooApplied = w.hairApplied = false; w.titleSet = -1; w.added = 0; w.eachPrice = 0; w.warning = false;
    w.recordGrew = w.recordNew = false;
    if (item.kind != NORMAL) w.paid += price;                 // cards and titles pay here (the title only if the def has the sub-def)
    if (item.kind == TATTOO) { w.tattooApplied = true; }
    else if (item.kind == HAIR) { w.hairApplied = true; }
    else if (item.kind == TITLE) { if (w.heroHasHeroTC) w.titleSet = def; }
    else {
        w.shopSold = count;
        w.paid += price;
        if (item.weapon && w.heroHasHeroTC && !w.weaponsUsable) w.warning = true;
        w.added = count; w.eachPrice = price / count;
        if (item.buyable) {
            switch (item.type) {
            case ITEMS:   w.totItems += count;   break;
            case WEAPONS: w.totWeapons += count; break;
            case CLOTHES: w.totClothes += count; break;
            }
            w.sellTotal = "Sell x " + Num(w.totItems + w.totWeapons + w.totClothes);
            bool found = false;
            for (size_t i = 0; i < w.sellRecords.size(); ++i) {
                if (w.sellRecords[i].count > 0 && def == w.sellRecords[i].def) {
                    w.sellRecords[i].count += count;
                    w.branch.subtitleCount += count;
                    found = true; w.recordGrew = true;
                    break;
                }
            }
            if (!found) {
                if (!w.branch.hasSubtitleRecord) { w.branch.hasSubtitleRecord = true; w.branch.subtitleCount = count; }
                else w.branch.subtitleCount += count;
                SellRecord r = { def, count }; w.sellRecords.push_back(r); w.recordNew = true;
                if (!w.branch.hasList) { w.branch.hasList = true; w.branch.listCreated = true; }
                w.branch.available = true;
            }
            w.branch.subtitle = "Branch" + (w.branch.subtitleCount != 0 ? " x " + Num(w.branch.subtitleCount) : std::string());
        }
    }
    w.money -= w.paid;
    w.arrowsRestored = 0;
    if (item.special) {
        for (std::map<long, Item>::iterator it = w.items.begin(); it != w.items.end(); ++it) {
            bool owned = false, hasArrow = false;
            for (size_t i = 0; i < alreadyOwnedSpecials.size(); ++i) if (alreadyOwnedSpecials[i] == it->first) owned = true;
            for (size_t i = 0; i < buyEntriesWithArrow.size(); ++i) if (buyEntriesWithArrow[i] == it->first) hasArrow = true;
            if (it->second.price <= w.money && !owned && !hasArrow) ++w.arrowsRestored;
        }
    }
    w.amountsRefreshed = !w.titleShop && !w.tattooShop && !w.barberShop;
    w.backEvent = true;
}

static World MakeWorld()
{
    World w;
    Item a = { 1, NORMAL, false, true, true, WEAPONS, 50 }; Item b = { 2, TATTOO, false, false, false, ITEMS, 200 };
    Item c = { 3, HAIR, false, false, false, ITEMS, 150 }; Item d = { 4, TITLE, true, false, false, ITEMS, 1000 };
    Item e = { 5, NORMAL, true, false, true, ITEMS, 300 }; Item f = { 6, NORMAL, false, false, true, CLOTHES, 20 };
    w.items[1] = a; w.items[2] = b; w.items[3] = c; w.items[4] = d; w.items[5] = e; w.items[6] = f;
    w.money = 2000; w.heroHasHeroTC = true; w.weaponsUsable = false; w.hasRoom = true;
    w.titleShop = w.tattooShop = w.barberShop = false;
    w.totItems = w.totWeapons = w.totClothes = 0;
    Branch br = { "", 0, false, false, false, false }; w.branch = br;
    return w;
}

int main()
{
    int ok = 1;
    std::vector<long> none;
    // a normal weapon, two of three in stock: the hero pays 100, the shop sells twice, the weapon warning is raised, the sell side gets
    // a new record under a branch with no list yet
    World w = MakeWorld();
    BuyItem(w, 1, 3, 2, none, none);
    if (w.stock != 1 || w.arrowRemoved || w.paid != 100 || w.shopSold != 2 || !w.warning || w.added != 2 || w.eachPrice != 50) ok = 0;
    if (w.totWeapons != 2 || w.sellTotal != "Sell x 2" || !w.recordNew || w.recordGrew || !w.branch.listCreated || !w.branch.available) ok = 0;
    if (w.branch.subtitle != "Branch x 2" || w.money != 1900 || !w.amountsRefreshed || !w.backEvent) ok = 0;
    // the same again with a record in stock: it grows, no new list
    w.branch.listCreated = false;
    BuyItem(w, 1, 1, 1, none, none);
    if (w.stock != 0 || !w.arrowRemoved || !w.recordGrew || w.recordNew || w.branch.listCreated || w.sellRecords.size() != 1 || w.sellRecords[0].count != 3) ok = 0;
    if (w.totWeapons != 3 || w.sellTotal != "Sell x 3" || w.branch.subtitle != "Branch x 3") ok = 0;
    // a weapon the hero can use raises no warning; no room for a normal item removes the arrow even with stock left
    w = MakeWorld(); w.weaponsUsable = true; w.hasRoom = false;
    BuyItem(w, 6, 5, 1, none, none);
    if (w.warning || !w.arrowRemoved || w.stock != 4 || w.totClothes != 1 || w.paid != 20) ok = 0;
    // a tattoo card: applied, paid once, nothing sold or added, no sell-side bookkeeping
    w = MakeWorld();
    BuyItem(w, 2, 1, 1, none, none);
    if (!w.tattooApplied || w.paid != 200 || w.shopSold != 0 || w.added != 0 || !w.sellRecords.empty() || w.totItems != 0) ok = 0;
    // a hair card likewise
    w = MakeWorld();
    BuyItem(w, 3, 1, 1, none, none);
    if (!w.hairApplied || w.paid != 150 || w.shopSold != 0) ok = 0;
    // a title (special): stock untouched, the hero's title set, other affordable not-owned specials without arrows get theirs back
    w = MakeWorld();
    std::vector<long> owned(1, 5), arrows(1, 2);
    BuyItem(w, 4, 1, 1, owned, arrows);
    if (w.stock != 1 || w.titleSet != 4 || w.paid != 1000 || w.money != 1000) ok = 0;
    if (w.arrowsRestored != 4) ok = 0;                       // items 1, 3, 4, 6 affordable at 1000; 5 is owned, 2 already has an arrow
    // without a hero component the title is not set; a title / tattoo / barber shop skips the entry amounts refresh
    w = MakeWorld(); w.heroHasHeroTC = false; w.titleShop = true;
    BuyItem(w, 4, 1, 1, none, none);
    if (w.titleSet != -1 || w.paid != 1000 || w.amountsRefreshed) ok = 0;
    if (ok) std::printf("005af64e_TEST PASS\n"); else std::printf("005af64e_TEST FAIL\n");
    return ok ? 0 : 1;
}