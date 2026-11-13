// Behaviour test for CTCInventoryTrade::ConstructTradeDescription (0x005AD571): a copy of the row's control flow over a mock UI
// (lines with a text and an RGB colour) and a mock shop / inventory / hero, checked on the special, buying and selling paths.
#include <cstdio>
#include <cstring>

struct Colour { unsigned char B, G, R; };
struct Line { char text[96]; Colour colour; };
struct Container { Line lines[16]; int count; };

struct Shop {
    long sellPrice, buyPrice, quantity, maxStock, days;
    bool barber, tattoo, title;
    long GetCurrentSellPrice(long, long) const { return sellPrice; }
    long GetCurrentBuyPrice(long, long) const { return buyPrice; }
    bool IsBarberShop() const { return barber; }
    bool IsTattooShop() const { return tattoo; }
    bool IsTitleShop() const { return title; }
    long GetQuantity(long) const { return quantity; }
    long GetMaxStock(long) const { return maxStock; }
    long GetDaysTillRestock(long) const { return days; }
};
struct Inventory { long owned, bought; };
struct Threshold { long stat, level; };
struct Hero { long statLevels[4]; Threshold thresholds[2]; int thresholdCount; };

static Line* NewLine(Container& c) { Line* l = &c.lines[c.count++]; l->text[0] = 0; l->colour.B = l->colour.G = l->colour.R = 200; return l; }
static void Red(Line* l) { l->colour.B = (unsigned char)(l->colour.B * 0.5f); l->colour.G = (unsigned char)(l->colour.G * 0.5f); }
static void Green(Line* l) { l->colour.B = (unsigned char)(l->colour.B * 0.5f); l->colour.R = (unsigned char)(l->colour.R * 0.5f); }

// value: the stock def's DefaultPrice (1 when there is none); heavyWeapon: the weapon block's condition (weapon def class 2)
static void Construct(Container& c, const Shop& shop, const Inventory& inv, const Hero& hero, long count, bool buying, bool special,
                      long value, bool heavyWeapon)
{
    c.count = 0;
    long price;
    if (buying && !special) price = shop.GetCurrentSellPrice(0, count);
    else price = shop.GetCurrentBuyPrice(0, count);
    Line* line = NewLine(c);
    std::sprintf(line->text, "%s: %ld (%ld)", special ? "Price" : "Cost", price, value);
    if (special) {
        if (price < value) Red(line); else if (price > value) Green(line);
        line = NewLine(c); std::sprintf(line->text, "desc");
        return;
    }
    if ((price > value && buying) || (price < value && !buying)) Red(line);
    else if ((price < value && buying) || (price > value && !buying)) Green(line);
    bool isSpecialShop = shop.IsBarberShop() || shop.IsTattooShop() || shop.IsTitleShop();
    if (buying) {
        if (!isSpecialShop) { line = NewLine(c); std::sprintf(line->text, "Owns: %ld", inv.owned); }
    } else {
        line = NewLine(c);
        long profit = price - inv.bought;
        if (profit >= 0) { std::sprintf(line->text, "Profit: %ld", profit); if (profit != 0) Green(line); }
        else { std::sprintf(line->text, "Loss: %ld", -profit); Red(line); }
    }
    if (!isSpecialShop) {
        line = NewLine(c); std::sprintf(line->text, "Stock: %ld/%ld", shop.GetQuantity(0), shop.GetMaxStock(0));
        line = NewLine(c);
        long days = shop.GetDaysTillRestock(0);
        if (days != 0) std::sprintf(line->text, "Delivery: %ld days", days); else std::sprintf(line->text, "Delivery: none");
    }
    if (buying && heavyWeapon) {
        for (int i = 0; i < hero.thresholdCount; ++i)
            if (hero.statLevels[hero.thresholds[i].stat] < hero.thresholds[i].level) {
                line = NewLine(c); std::sprintf(line->text, "Stat%ld level %ld required", hero.thresholds[i].stat, hero.thresholds[i].level);
                Red(line);
            }
    }
    line = NewLine(c); std::sprintf(line->text, "desc");
}

int main()
{
    bool ok = true;
    Container c;
    Shop shop = { 120, 80, 3, 10, 2, false, false, false };
    Inventory inv = { 1, 100 };
    Hero hero = { { 1, 5, 0, 0 }, { { 0, 3 }, { 1, 2 } }, 2 };
    // special: buy price 80 < value 100 -> red price line + description
    Construct(c, shop, inv, hero, 1, true, true, 100, false);
    ok = ok && c.count == 2 && std::strcmp(c.lines[0].text, "Price: 80 (100)") == 0 && c.lines[0].colour.B == 100 && c.lines[0].colour.G == 100 && c.lines[0].colour.R == 200;
    // buying at 120 > 100 -> red; owns, stock, delivery, one unmet heavy-weapon stat (stat 0: level 1 < 3), description
    Construct(c, shop, inv, hero, 1, true, false, 100, true);
    ok = ok && c.count == 6 && c.lines[0].colour.G == 100 && std::strcmp(c.lines[1].text, "Owns: 1") == 0
            && std::strcmp(c.lines[2].text, "Stock: 3/10") == 0 && std::strcmp(c.lines[3].text, "Delivery: 2 days") == 0
            && std::strcmp(c.lines[4].text, "Stat0 level 3 required") == 0 && c.lines[4].colour.G == 100;
    // selling at 80 < 100 bought -> loss 20 in red; price line red (price < value, selling); a title shop has no stock lines
    shop.title = true;
    Construct(c, shop, inv, hero, 1, false, false, 100, true);
    ok = ok && c.count == 3 && std::strcmp(c.lines[1].text, "Loss: 20") == 0 && c.lines[1].colour.G == 100 && c.lines[0].colour.G == 100;
    // selling at a profit: green profit line; no delivery days
    shop.title = false; shop.buyPrice = 150; shop.days = 0;
    Construct(c, shop, inv, hero, 1, false, false, 100, false);
    ok = ok && c.count == 5 && std::strcmp(c.lines[1].text, "Profit: 50") == 0 && c.lines[1].colour.R == 100 && c.lines[1].colour.G == 200
            && std::strcmp(c.lines[3].text, "Delivery: none") == 0 && c.lines[0].colour.R == 100;
    if (!ok) { std::printf("005ad571_TEST FAIL\n"); return 1; }
    std::printf("005ad571_TEST PASS\n");
    return 0;
}
