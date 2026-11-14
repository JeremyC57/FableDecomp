// Behaviour test for 0x0058807D CTCInventory::OpenPCInventoryMenu(): the menu state becomes 3; an open PC_INVENTORY_LIST component
// is reused, else one is built: the frame (when present) positions the viewport, title, gold amount (set to the hero's money) and the
// pictures / descriptions / subtitles (the three style descriptions also become viewport children); the owned items (books dropped,
// the hero's weapons and the clothing inventory appended) are counted per menu entry (augmentations skipped; weapons; a hero suit
// counts as clothing when a part is owned and either the suit is the naked suit or the part is none of the five "no clothing"
// objects; appearance modifiers with suit part 1..5; expressions skipped; hair cards by covered body area into facial hair (0x80 /
// 0x100) or haircuts (0x10); valid tattoo cards; everything else that is not an ability as an item); each entry's description joins
// the viewport, an entry with its inventory present and a positive count gets "<text>x<count>" on its button text and subtitle and a
// construct action (69 / 68 / 70 with entry, list; 240 with list, entry, haircuts, facial hair, tattoos), else it is made
// unavailable; the list is initialised and appended; then the scroll buttons observe event 25, back / close are added as manager
// observers and observe event 25, and the list goes to state 5.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name, value;
    std::map<std::string, Comp*> children;
    std::vector<Comp*> positioned, fake;
    std::vector<int> observed;
    int actionType; std::vector<Comp*> actionComps; std::vector<long> actionLongs;
    int state; bool initialised, managerObserver, unavailable;
    Comp(const char* n) : name(n), actionType(0), state(0), initialised(false), managerObserver(false), unavailable(false) {}
    Comp* FindChild(const char* n) { std::map<std::string, Comp*>::iterator it = children.find(n); return it == children.end() ? 0 : it->second; }
};
// item definition kinds of the mock definition manager
struct Def {
    bool book, augmentation, weapon, expression, ability, invalidTattoo, tattoo;
    std::vector<long> suitParts;      // a hero suit when not empty
    long appearanceSuitPart;          // an appearance modifier when >= 0
    long hairFlags;                   // a hair card when >= 0 (the covered body areas of its hair object)
    Def() : book(false), augmentation(false), weapon(false), expression(false), ability(false), invalidTattoo(false), tattoo(false),
            appearanceSuitPart(-1), hairFlags(-1) {}
};
enum { NAKED = 100, NO_HAT, NO_SHIRT, NO_GLOVES, NO_TROUSERS, NO_BOOTS };
typedef std::vector<std::pair<long, long> > Items;
static std::string num(long v) { char b[16]; std::sprintf(b, "%ld", v); return b; }

struct Inventory {
    int menu; std::vector<Comp*> components; Comp* built; int created; long money;
    std::map<long, Def> defs;
    Items own, heroWeapons, clothingItems;
    bool hasWeapons, hasInventory, hasClothing;
    long CountOwned(long type) { long n = 0; for (size_t k = 0; k < clothingItems.size(); ++k) if (clothingItems[k].first == type) n += clothingItems[k].second; return n; }
    void Entry(Comp* list, Comp* viewport, const char* entryName, const char* desc, const char* button, const char* text, const char* subtitle,
               bool present, long count, int type, bool style, long h, long f, long t)
    {
        Comp* entry = list->FindChild(entryName);
        viewport->fake.push_back(entry->FindChild(desc));
        if (present && count > 0) {
            Comp* label = entry->FindChild(button)->FindChild(text);
            std::string s = label->value + "x" + num(count);
            label->value = s;
            Comp* sub = entry->FindChild(subtitle);
            if (sub) sub->value = s;
            entry->actionType = type;
            if (style) { entry->actionComps.push_back(list); entry->actionComps.push_back(entry); entry->actionLongs.push_back(h); entry->actionLongs.push_back(f); entry->actionLongs.push_back(t); }
            else { entry->actionComps.push_back(entry); entry->actionComps.push_back(list); }
        } else
            entry->unavailable = true;
    }
    void OpenPCInventoryMenu()
    {
        menu = 3;
        Comp* list = 0;
        for (unsigned long i = 0; i < components.size(); ++i) {
            if (list) break;
            if (components[i]->name == "PC_INVENTORY_LIST") list = components[i];
        }
        if (!list) {
            list = built; ++created;
            Comp* frame = list->FindChild("PC_UI_FRAME");
            if (frame) {
                Comp* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");
                if (viewport) frame->positioned.push_back(viewport);
                Comp* child = list->FindChild("PC_TITLE_INVENTORY");
                if (child) frame->positioned.push_back(child);
                Comp* gold = list->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN_TOP_LEVEL");
                if (gold) { gold->value = num(money); frame->positioned.push_back(gold); }
                static const char* const names[] = { "PC_GOLD_COIN_SPRITE_TOP_LEVEL", "PC_RING_PIC_WEAPONS", "PC_WEAPON_DESC", "PC_WEAPON_SUBTITLE",
                    "PC_RING_PIC_ITEMS", "PC_ITEMS_DESC", "PC_ITEMS_SUBTITLE", "PC_RING_PIC_CLOTHING", "PC_CLOTHING_DESC", "PC_CLOTHING_SUBTITLE",
                    "PC_RING_PIC_STYLE_CARDS", "PC_STYLE_CARDS_DESC", "PC_STYLE_CARDS_SUBTITLE", "PC_STYLE_CARDS_TITLE", "PC_RING_PIC_HAIRCUTS",
                    "PC_HAIRCUTS_DESC", "PC_HAIRCUTS_SUBTITLE", "PC_RING_PIC_FACIAL_HAIR", "PC_FACIAL_HAIR_DESC", "PC_FACIAL_HAIR_SUBTITLE",
                    "PC_RING_PIC_TATTOOS", "PC_TATTOOS_DESC", "PC_TATTOOS_SUBTITLE" };
                for (int k = 0; k < 23; ++k) {
                    child = list->FindChild(names[k]);
                    if (child) {
                        frame->positioned.push_back(child);
                        if (k == 15 || k == 18 || k == 21) viewport->fake.push_back(child);
                    }
                }
            }
            Items items = own;
            for (Items::iterator it = items.begin(); it != items.end(); ) {
                if (defs[it->first].book) it = items.erase(it); else ++it;
            }
            items.insert(items.end(), heroWeapons.begin(), heroWeapons.end());
            if (hasClothing) items.insert(items.end(), clothingItems.begin(), clothingItems.end());
            long weaponsCount = 0, itemsCount = 0, clothingCount = 0, haircutCount = 0, facialHairCount = 0, tattooCount = 0;
            for (unsigned long i = 0; i < items.size(); ++i) {
                const Def& d = defs[items[i].first];
                if (d.augmentation) continue;
                if (d.weapon) weaponsCount += items[i].second;
                else if (!d.suitParts.empty()) {
                    for (size_t p = 0; p < d.suitParts.size(); ++p) {
                        long part = d.suitParts[p];
                        if (CountOwned(part) > 0) {
                            if (items[i].first == NAKED || (part != NO_HAT && part != NO_SHIRT && part != NO_GLOVES && part != NO_TROUSERS && part != NO_BOOTS)) {
                                clothingCount += items[i].second;
                                break;
                            }
                        }
                    }
                } else if (d.appearanceSuitPart >= 0) {
                    if (d.appearanceSuitPart > 0 && d.appearanceSuitPart <= 5) clothingCount += items[i].second;
                } else if (!d.expression) {
                    bool hair = d.hairFlags >= 0;
                    if (!hair && !d.tattoo) {
                        if (!d.ability) itemsCount += items[i].second;
                    } else if (hair) {
                        if ((d.hairFlags & 0x80) || (d.hairFlags & 0x100)) facialHairCount += items[i].second;
                        else if (d.hairFlags & 0x10) haircutCount += items[i].second;
                    } else if (!d.invalidTattoo)
                        tattooCount += items[i].second;
                }
            }
            Comp* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");
            Entry(list, viewport, "PC_MENU_ENTRY_CLOTHING", "PC_CLOTHING_DESC", "PC_CLOTHING_BUTTON", "PC_TEXT_CLOTHING", "PC_CLOTHING_SUBTITLE", hasClothing, clothingCount, 69, false, 0, 0, 0);
            Entry(list, viewport, "PC_MENU_ENTRY_WEAPONS", "PC_WEAPON_DESC", "PC_WEAPONS_BUTTON", "PC_TEXT_WEAPONS", "PC_WEAPON_SUBTITLE", hasWeapons, weaponsCount, 68, false, 0, 0, 0);
            Entry(list, viewport, "PC_MENU_ENTRY_ITEMS", "PC_ITEMS_DESC", "PC_ITEMS_BUTTON", "PC_TEXT_ITEMS", "PC_ITEMS_SUBTITLE", hasInventory, itemsCount, 70, false, 0, 0, 0);
            Entry(list, viewport, "PC_MENU_ENTRY_STYLE_CARDS", "PC_STYLE_CARDS_DESC", "PC_STYLE_CARDS_BUTTON", "PC_TEXT_STYLE_CARDS", "PC_STYLE_CARDS_SUBTITLE",
                  hasInventory, facialHairCount + tattooCount + haircutCount, 240, true, haircutCount, facialHairCount, tattooCount);
            list->initialised = true;
            components.push_back(list);
        }
        static const char* const scrolls[] = { "SCROLL_LIST_UP", "SCROLL_DESCRIPTION_UP", "SCROLL_LIST_DOWN", "SCROLL_DESCRIPTION_DOWN" };
        for (int k = 0; k < 4; ++k) { Comp* s = list->FindChild(scrolls[k]); if (s) s->observed.push_back(25); }
        static const char* const clicks[] = { "PC_BACK_CLICKABLE", "PC_CLOSE_CLICKABLE" };
        for (int k = 0; k < 2; ++k) { Comp* c = list->FindChild(clicks[k]); if (c) { c->managerObserver = true; c->observed.push_back(25); } }
        list->state = 5;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
struct Screen {
    Comp list, frame, view, title, gold, hairDesc, tatDesc;
    Comp ec, ew, ei, es, dc, dw, di, ds, bc, bw, bi, bs, tc, tw, ti, ts, sc, ss, up, back, close;
    Screen() : list("PC_INVENTORY_LIST"), frame("PC_UI_FRAME"), view("PC_DESCRIPTION_VIEWPORT"), title("PC_TITLE_INVENTORY"),
        gold("PC_GOLD_AMOUNT_TRADE_SCREEN_TOP_LEVEL"), hairDesc("PC_HAIRCUTS_DESC"), tatDesc("PC_TATTOOS_DESC"),
        ec("PC_MENU_ENTRY_CLOTHING"), ew("PC_MENU_ENTRY_WEAPONS"), ei("PC_MENU_ENTRY_ITEMS"), es("PC_MENU_ENTRY_STYLE_CARDS"),
        dc("PC_CLOTHING_DESC"), dw("PC_WEAPON_DESC"), di("PC_ITEMS_DESC"), ds("PC_STYLE_CARDS_DESC"),
        bc("PC_CLOTHING_BUTTON"), bw("PC_WEAPONS_BUTTON"), bi("PC_ITEMS_BUTTON"), bs("PC_STYLE_CARDS_BUTTON"),
        tc("PC_TEXT_CLOTHING"), tw("PC_TEXT_WEAPONS"), ti("PC_TEXT_ITEMS"), ts("PC_TEXT_STYLE_CARDS"),
        sc("PC_CLOTHING_SUBTITLE"), ss("PC_STYLE_CARDS_SUBTITLE"), up("SCROLL_LIST_UP"), back("PC_BACK_CLICKABLE"), close("PC_CLOSE_CLICKABLE")
    {
        Comp* top[] = { &view, &title, &gold, &hairDesc, &tatDesc, &ec, &ew, &ei, &es, &up, &back, &close };
        for (int k = 0; k < 12; ++k) list.children[top[k]->name] = top[k];
        ec.children[dc.name] = &dc; ec.children[bc.name] = &bc; bc.children[tc.name] = &tc; ec.children[sc.name] = &sc;
        ew.children[dw.name] = &dw; ew.children[bw.name] = &bw; bw.children[tw.name] = &tw;
        ei.children[di.name] = &di; ei.children[bi.name] = &bi; bi.children[ti.name] = &ti;
        es.children[ds.name] = &ds; es.children[bs.name] = &bs; bs.children[ts.name] = &ts; es.children[ss.name] = &ss;
        tc.value = "Clothing "; tw.value = "Weapons "; ti.value = "Items "; ts.value = "Cards ";
    }
};
static void fill(Inventory& inv)
{
    inv.menu = 0; inv.created = 0; inv.money = 1234; inv.hasWeapons = inv.hasInventory = inv.hasClothing = true;
    inv.defs[1].book = true; inv.defs[2].augmentation = true; inv.defs[3].weapon = true; inv.defs[4].expression = true;
    inv.defs[5].ability = true; inv.defs[6];                                   // 6: a plain item
    inv.defs[7].hairFlags = 0x10; inv.defs[8].hairFlags = 0x80; inv.defs[9].hairFlags = 0x100; inv.defs[10].hairFlags = 0x1;
    inv.defs[11].tattoo = true; inv.defs[12].tattoo = true; inv.defs[12].invalidTattoo = true;
    inv.defs[13].appearanceSuitPart = 5; inv.defs[14].appearanceSuitPart = 6; inv.defs[15].appearanceSuitPart = 0;
    inv.defs[20].suitParts.push_back(NO_HAT); inv.defs[20].suitParts.push_back(50);      // a suit: a "no hat" part and a real part 50
    inv.defs[21].suitParts.push_back(NO_BOOTS);                                          // only a "no boots" part
    inv.defs[NAKED].suitParts.push_back(NO_SHIRT);                                       // the naked suit counts with any owned part
    inv.defs[22].suitParts.push_back(51);                                                // its part is not owned
    Items& o = inv.own;
    o.push_back(std::make_pair(1L, 9L)); o.push_back(std::make_pair(2L, 9L)); o.push_back(std::make_pair(3L, 2L)); o.push_back(std::make_pair(4L, 9L));
    o.push_back(std::make_pair(5L, 9L)); o.push_back(std::make_pair(6L, 3L)); o.push_back(std::make_pair(7L, 1L)); o.push_back(std::make_pair(8L, 2L));
    o.push_back(std::make_pair(9L, 4L)); o.push_back(std::make_pair(10L, 9L)); o.push_back(std::make_pair(11L, 5L)); o.push_back(std::make_pair(12L, 9L));
    inv.heroWeapons.push_back(std::make_pair(3L, 1L));
    Items& c = inv.clothingItems;
    c.push_back(std::make_pair(13L, 1L)); c.push_back(std::make_pair(14L, 9L)); c.push_back(std::make_pair(15L, 9L)); c.push_back(std::make_pair(20L, 2L));
    c.push_back(std::make_pair(21L, 9L)); c.push_back(std::make_pair((long)NAKED, 4L)); c.push_back(std::make_pair(22L, 9L));
    c.push_back(std::make_pair(50L, 1L)); c.push_back(std::make_pair((long)NO_BOOTS, 1L)); c.push_back(std::make_pair((long)NO_SHIRT, 1L)); c.push_back(std::make_pair((long)NO_HAT, 1L));
    inv.defs[50].appearanceSuitPart = 9; inv.defs[NO_BOOTS].appearanceSuitPart = 9; inv.defs[NO_SHIRT].appearanceSuitPart = 9; inv.defs[NO_HAT].appearanceSuitPart = 9;
}
int main()
{
    {   // full build with a frame
        Screen s; s.list.children["PC_UI_FRAME"] = &s.frame;
        Comp other("OTHER");
        Inventory inv; fill(inv); inv.built = &s.list; inv.components.push_back(&other);
        inv.OpenPCInventoryMenu();
        check(inv.menu == 3 && inv.created == 1 && inv.components.size() == 2 && s.list.initialised, "built and kept");
        check(s.frame.positioned.size() == 5 && s.frame.positioned[0] == &s.view && s.frame.positioned[1] == &s.title && s.frame.positioned[2] == &s.gold
              && s.frame.positioned[3] == &s.hairDesc && s.frame.positioned[4] == &s.tatDesc, "frame children");
        check(s.gold.value == "1234", "gold amount");
        check(s.view.fake.size() == 6 && s.view.fake[0] == &s.hairDesc && s.view.fake[1] == &s.tatDesc && s.view.fake[2] == &s.dc && s.view.fake[3] == &s.dw
              && s.view.fake[4] == &s.di && s.view.fake[5] == &s.ds, "viewport children");
        // clothing: 13 (part 5) 1 + suit 20 (real part 50 owned) 2 + naked suit 4 = 7; 14, 15, 21 (only a no-boots part), 22 not counted
        check(s.tc.value == "Clothing x7" && s.sc.value == "Clothing x7" && s.ec.actionType == 69 && s.ec.actionComps[0] == &s.ec && s.ec.actionComps[1] == &s.list && !s.ec.unavailable, "clothing entry");
        check(s.tw.value == "Weapons x3" && s.ew.actionType == 68 && s.ew.actionComps[0] == &s.ew, "weapons entry");   // own 2 + hero 1
        check(s.ti.value == "Items x3" && s.ei.actionType == 70 && s.ei.actionComps[1] == &s.list, "items entry");       // only item 6
        // haircuts 1 (0x10), facial hair 2 + 4 (0x80, 0x100), tattoos 5 (the invalid one dropped): 12 cards
        check(s.ts.value == "Cards x12" && s.ss.value == "Cards x12" && s.es.actionType == 240 && s.es.actionComps[0] == &s.list && s.es.actionComps[1] == &s.es
              && s.es.actionLongs.size() == 3 && s.es.actionLongs[0] == 1 && s.es.actionLongs[1] == 6 && s.es.actionLongs[2] == 5, "style cards entry");
        check(s.up.observed.size() == 1 && s.up.observed[0] == 25 && s.back.managerObserver && s.back.observed.size() == 1 && s.close.managerObserver && s.list.state == 5, "observers and state");
        inv.OpenPCInventoryMenu();   // the second open reuses the list
        check(inv.created == 1 && inv.components.size() == 2 && s.up.observed.size() == 2 && s.tc.value == "Clothing x7", "reused");
    }
    {   // no frame, no clothing inventory, no item inventory: only the weapons entry stays available
        Screen s;
        Inventory inv; fill(inv); inv.built = &s.list; inv.hasClothing = false; inv.hasInventory = false;
        inv.OpenPCInventoryMenu();
        check(s.frame.positioned.empty() && s.gold.value.empty() && s.view.fake.size() == 4, "no frame");
        check(s.ec.unavailable && s.ec.actionType == 0 && s.tc.value == "Clothing ", "clothing unavailable");
        check(!s.ew.unavailable && s.tw.value == "Weapons x3", "weapons available");
        check(s.ei.unavailable && s.es.unavailable && s.es.actionLongs.empty(), "items and cards unavailable");
    }
    {   // inventories present but nothing countable: every entry unavailable
        Screen s;
        Inventory inv; fill(inv); inv.built = &s.list; inv.own.clear(); inv.heroWeapons.clear(); inv.clothingItems.clear();
        inv.own.push_back(std::make_pair(1L, 3L)); inv.own.push_back(std::make_pair(2L, 3L)); inv.own.push_back(std::make_pair(4L, 3L)); inv.own.push_back(std::make_pair(5L, 3L));
        inv.own.push_back(std::make_pair(10L, 3L)); inv.own.push_back(std::make_pair(12L, 3L));
        inv.OpenPCInventoryMenu();
        check(s.ec.unavailable && s.ew.unavailable && s.ei.unavailable && s.es.unavailable && s.list.initialised && s.list.state == 5, "nothing to show");
    }
    if (fails) return 1;
    std::printf("0058807d_TEST PASS\n");
    return 0;
}
