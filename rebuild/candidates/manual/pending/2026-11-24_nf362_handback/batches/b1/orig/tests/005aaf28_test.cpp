// Behaviour test for CTCInventoryTrade::ConstructTradeDescriptionClothing (0x005AAF28): a copy of the row's logic over small models of
// the shop, the hero's inventory and the UI text lines (each line keeps its text and its B/G/R colour bytes).
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Line { std::string text; unsigned char B, G, R; Line() : B(200), G(200), R(200) {} };
struct Shop { long sell, buy, quantity, maxStock, days; bool barber, tattoo, title; };
struct Inventory { bool clothing; long bought, owned; std::string armour, noArmour, description; };
struct Hero { std::map<int, long> thresholds; std::map<int, long> levels; };

static std::string Int(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static void Halve(unsigned char& c) { float f = (float)c; f *= 0.5f; c = (unsigned char)(long)f; }

// The row's logic: returns the lines added to the container.
static std::vector<Line> Construct(const Shop& shop, const Inventory& inv, const Hero& hero, long defaultPrice,
                                   bool buying, bool special, bool heavyWeapon)
{
    std::vector<Line> out;
    long price = (buying && !special) ? shop.sell : shop.buy;
    Line line;
    if (special) {
        line.text = "Price: " + Int(price) + " (" + Int(defaultPrice) + ")";
        out.push_back(line);
        if (price < defaultPrice) { Halve(out.back().B); Halve(out.back().G); }
        else if (price > defaultPrice) { Halve(out.back().B); Halve(out.back().R); }
        Line d; d.text = inv.description; out.push_back(d);
        return out;
    }
    line.text = "Cost: " + Int(price) + " (" + Int(defaultPrice) + ")";
    out.push_back(line);
    if ((price > defaultPrice && buying) || (price < defaultPrice && !buying)) { Halve(out.back().B); Halve(out.back().G); }
    else if ((price < defaultPrice && buying) || (price > defaultPrice && !buying)) { Halve(out.back().B); Halve(out.back().R); }
    bool specialShop = shop.barber || shop.tattoo || shop.title;
    if (!buying) {
        Line p;
        long profit = price - inv.bought;
        if (profit >= 0) {
            p.text = "Profit: " + Int(profit);
            if (profit != 0) { Halve(p.B); Halve(p.R); }
        } else {
            p.text = "Loss: " + Int(-profit);
            Halve(p.B); Halve(p.G);
        }
        out.push_back(p);
    }
    if (inv.clothing && inv.armour != "") { Line a; a.text = inv.armour; out.push_back(a); }
    if (buying && !specialShop) { Line o; o.text = "Owned: " + Int(inv.owned); out.push_back(o); }
    if (!specialShop) {
        Line s; s.text = "In stock: " + Int(shop.quantity) + "/" + Int(shop.maxStock); out.push_back(s);
        Line d; d.text = "Delivery: ";
        if (shop.days != 0) d.text += Int(shop.days) + " days"; else d.text += "none";
        out.push_back(d);
    }
    if (buying && heavyWeapon) {
        for (std::map<int, long>::const_iterator it = hero.thresholds.begin(); it != hero.thresholds.end(); ++it) {
            std::map<int, long>::const_iterator lv = hero.levels.find(it->first);
            long level = lv == hero.levels.end() ? 0 : lv->second;
            if (level < it->second) {
                Line r; r.text = "Stat" + Int(it->first) + " level " + Int(it->second) + " required";
                Halve(r.B); Halve(r.G); out.push_back(r);
            }
        }
    }
    if (inv.clothing) { Line n; n.text = inv.noArmour; out.push_back(n); }
    return out;
}

int main()
{
    Shop shop = { 120, 80, 3, 5, 0, false, false, false };
    Inventory inv = { true, 50, 2, "Armour 4", "Plain", "A hat" };
    Hero hero; hero.thresholds[1] = 3; hero.thresholds[2] = 1; hero.levels[1] = 2; hero.levels[2] = 5;
    // buying, normal shop, price above default: cost line tinted (B,G halved), owned, stock, delivery "none", threshold line, no-armour
    std::vector<Line> a = Construct(shop, inv, hero, 100, true, false, true);
    if (a.size() != 7 || a[0].text != "Cost: 120 (100)" || a[0].B != 100 || a[0].G != 100 || a[0].R != 200
        || a[2].text != "Owned: 2" || a[3].text != "In stock: 3/5" || a[4].text != "Delivery: none"
        || a[5].text != "Stat1 level 3 required" || a[6].text != "Plain") { std::printf("005aaf28_TEST FAIL\n"); return 1; }
    // selling for more than bought: profit line tinted B,R; no owned / no heavy line; delivery days
    shop.days = 2;
    std::vector<Line> b = Construct(shop, inv, hero, 100, false, false, true);
    if (b.size() != 6 || b[0].text != "Cost: 80 (100)" || b[0].B != 100 || b[0].G != 100 || b[1].text != "Profit: 30"
        || b[1].R != 100 || b[4].text != "Delivery: 2 days") { std::printf("005aaf28_TEST FAIL\n"); return 1; }
    // selling for less than bought (bought 95, sells at 80): loss line with -profit, tinted B,G, R untouched
    inv.bought = 95;
    std::vector<Line> l = Construct(shop, inv, hero, 100, false, false, true);
    if (l.size() != 6 || l[1].text != "Loss: 15" || l[1].B != 100 || l[1].G != 100 || l[1].R != 200) {
        std::printf("005aaf28_TEST FAIL\n"); return 1;
    }
    // selling at exactly the bought price: profit 0 is a profit line, untinted
    inv.bought = 80;
    std::vector<Line> z = Construct(shop, inv, hero, 100, false, false, true);
    if (z.size() != 6 || z[1].text != "Profit: 0" || z[1].B != 200 || z[1].G != 200 || z[1].R != 200) {
        std::printf("005aaf28_TEST FAIL\n"); return 1;
    }
    inv.bought = 50;
    // special item cheaper than default: price tinted B,G, then the item description only
    std::vector<Line> c = Construct(shop, inv, hero, 100, true, true, false);
    if (c.size() != 2 || c[0].text != "Price: 80 (100)" || c[0].G != 100 || c[0].R != 200 || c[1].text != "A hat") {
        std::printf("005aaf28_TEST FAIL\n"); return 1;
    }
    // barber shop: no owned / stock / delivery lines
    shop.barber = true;
    std::vector<Line> d = Construct(shop, inv, hero, 120, true, false, false);
    if (d.size() != 3 || d[0].B != 200) { std::printf("005aaf28_TEST FAIL\n"); return 1; }
    std::printf("005aaf28_TEST PASS\n");
    return 0;
}