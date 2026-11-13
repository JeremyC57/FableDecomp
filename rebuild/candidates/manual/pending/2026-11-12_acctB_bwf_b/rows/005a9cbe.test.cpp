// Behaviour test for CTCInventoryTrade::ConstructTradeDescriptionWeapon (0x005A9CBE): the same decision logic over a small model of
// the UI components, shop, inventories and hero experience (strings are narrow std::string; colours are bytes faded by *= 0.5f).
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp { std::string tmpl, text; unsigned char B, G, R; std::vector<Comp*> kids; Comp(const char* t) : tmpl(t), B(200), G(200), R(200) {} };
static std::vector<Comp*> g_all;
static Comp* Create(const char* t) { Comp* c = new Comp(t); g_all.push_back(c); return c; }
static std::string Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

struct Shop { long sell, buy, qty, maxq, days; bool special; };
struct Inv { bool weapons; long bought, owned; };
struct World {
    bool pcMode; Shop shop; Inv inv; long defaultPrice; bool hasStockDef; bool isWeapon; long weaponClass;
    std::map<int, long> thresholds; std::map<int, long> levels;
};

static Comp* Construct(World& w, long item, bool selling, bool brief)
{
    Comp* textComponent;
    Comp* description;
    if (w.pcMode) { description = Create("PC_DESC"); textComponent = Create("PC_TEXT"); }
    else          { description = Create("UI_DESC"); textComponent = Create("UI_TEXT"); }
    std::string text;
    Comp* line = textComponent;
    long price = (selling && !brief) ? w.shop.sell : w.shop.buy;
    long defaultPrice = w.hasStockDef ? w.defaultPrice : 1;
    if (brief) {
        text = "Price: " + Num(price);
        text += " (" + Num(defaultPrice) + ")";
        description->kids.push_back(textComponent); line->text = text;
        if (price < defaultPrice) { textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->G = (unsigned char)(textComponent->G * 0.5f); }
        else if (price > defaultPrice) { textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->R = (unsigned char)(textComponent->R * 0.5f); }
        textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
        text = "";
        text = "desc" + Num(item);
        description->kids.push_back(textComponent); line->text = text;
    } else {
        text = "Cost: " + Num(price);
        text += " (" + Num(defaultPrice) + ")";
        description->kids.push_back(textComponent); line->text = text;
        if ((price > defaultPrice && selling) || (price < defaultPrice && !selling)) {
            textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->G = (unsigned char)(textComponent->G * 0.5f);
        } else if ((price < defaultPrice && selling) || (price > defaultPrice && !selling)) {
            textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->R = (unsigned char)(textComponent->R * 0.5f);
        }
        bool special = w.shop.special;
        if (!selling) {
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            long profit = price - w.inv.bought;
            if (profit >= 0) {
                text = "Profit: " + Num(profit);
                if (profit != 0) { textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->R = (unsigned char)(textComponent->R * 0.5f); }
            } else {
                text = "Loss: " + Num(-profit);
                textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->G = (unsigned char)(textComponent->G * 0.5f);
            }
            description->kids.push_back(textComponent); line->text = text;
        }
        if (w.inv.weapons) {
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            text = "Damage"; description->kids.push_back(textComponent); line->text = text;
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            text = "Weight"; description->kids.push_back(textComponent); line->text = text;
        }
        if (selling && !special) {
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            text = "Owns: " + Num(w.inv.owned); description->kids.push_back(textComponent); line->text = text;
        }
        if (!special) {
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            text = "Stock: " + Num(w.shop.qty); text += "/" + Num(w.shop.maxq);
            description->kids.push_back(textComponent); line->text = text;
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            text = "Delivery: ";
            if (w.shop.days != 0) text += Num(w.shop.days) + " days"; else text += "none";
            description->kids.push_back(textComponent); line->text = text;
        }
        if (selling && w.isWeapon && w.weaponClass == 2) {
            for (std::map<int, long>::const_iterator it = w.thresholds.begin(); it != w.thresholds.end(); ++it) {
                if (w.levels[it->first] < it->second) {
                    textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
                    text = "Stat" + Num(it->first) + " level " + Num(it->second) + " required";
                    textComponent->B = (unsigned char)(textComponent->B * 0.5f); textComponent->G = (unsigned char)(textComponent->G * 0.5f);
                    description->kids.push_back(textComponent); line->text = text;
                }
            }
        }
        if (w.inv.weapons) {
            textComponent = Create(w.pcMode ? "PC_TEXT" : "UI_TEXT"); line = textComponent;
            text = "Class"; description->kids.push_back(textComponent); line->text = text;
        }
    }
    return description;
}

static World Base()
{
    World w; w.pcMode = true; w.shop.sell = 120; w.shop.buy = 80; w.shop.qty = 3; w.shop.maxq = 5; w.shop.days = 2; w.shop.special = false;
    w.inv.weapons = true; w.inv.bought = 90; w.inv.owned = 1; w.defaultPrice = 100; w.hasStockDef = true; w.isWeapon = true; w.weaponClass = 2;
    w.thresholds[1] = 3; w.thresholds[2] = 1; w.levels[1] = 2; w.levels[2] = 5;
    return w;
}

int main()
{
    int bad = 0;
    // brief: price line + description, price under default -> faded B and G (red)
    { World w = Base(); Comp* d = Construct(w, 7, false, true);
      if (d->tmpl != "PC_DESC" || d->kids.size() != 2 || d->kids[0]->text != "Price: 80 (100)" || d->kids[0]->B != 100 || d->kids[0]->G != 100 || d->kids[0]->R != 200 || d->kids[1]->text != "desc7") bad = 1; }
    // selling (hero buys) above default -> red price; owns, stock, delivery, one unmet heavy threshold, class line
    { World w = Base(); Comp* d = Construct(w, 7, true, false);
      if (d->kids.size() != 8 || d->kids[0]->text != "Cost: 120 (100)" || d->kids[0]->G != 100 || d->kids[3]->text != "Owns: 1" ||
          d->kids[4]->text != "Stock: 3/5" || d->kids[5]->text != "Delivery: 2 days" || d->kids[6]->text != "Stat1 level 3 required" || d->kids[7]->text != "Class") bad = 2; }
    // hero sells at a loss in a special console shop: no stock lines, loss line faded red
    { World w = Base(); w.pcMode = false; w.shop.special = true; w.inv.bought = 95; Comp* d = Construct(w, 7, false, false);
      if (d->tmpl != "UI_DESC" || d->kids.size() != 5 || d->kids[0]->G != 100 || d->kids[0]->R != 200 || d->kids[1]->text != "Loss: 15" || d->kids[1]->G != 100) bad = 3; }
    // no stock def -> default price 1, no delivery
    { World w = Base(); w.hasStockDef = false; w.shop.days = 0; w.inv.weapons = false; w.weaponClass = 1; Comp* d = Construct(w, 3, true, false);
      if (d->kids.size() != 4 || d->kids[0]->text != "Cost: 120 (1)" || d->kids[3]->text != "Delivery: none") bad = 4; }
    for (size_t i = 0; i < g_all.size(); ++i) delete g_all[i];
    if (bad) { std::printf("005a9cbe_TEST FAIL\n"); return 1; }
    std::printf("005a9cbe_TEST PASS\n");
    return 0;
}
