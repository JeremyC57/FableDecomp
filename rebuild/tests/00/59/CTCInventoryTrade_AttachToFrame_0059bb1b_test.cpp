// Behaviour test for CTCInventoryTrade::AttachToFrame @ 0x0059BB1B: with an inactive frame present, the gold readout, the coin
// sprite, the three category rings and the 13 sub-category rings are attached to it in retail order; the six buttons that exist
// are registered with the UI manager and observe mouse movement (event 25); missing buttons are skipped.
#include <cstdio>
#include <string>
#include <vector>
#include <map>
struct Comp {
    std::string name; std::map<std::string, Comp*> kids; std::vector<std::string> positionChildren; std::vector<int> events;
    Comp* FindChild(const std::string& n) { return kids.count(n) ? kids[n] : 0; }
    void AddPositionChild(Comp* c) { positionChildren.push_back(c ? c->name : std::string("<null>")); }
    void ObserveEvent(int e) { events.push_back(e); }
};
static std::vector<Comp*> gObservers;
static Comp* Add(Comp* parent, const std::string& n) { Comp* c = new Comp; c->name = n; parent->kids[n] = c; return c; }
static void AttachToFrame(Comp* menu)
{
    Comp* frame = menu->FindChild("PC_UI_INACTIVE_FRAME");
    if (frame) {
        frame->AddPositionChild(menu->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"));
        frame->AddPositionChild(menu->FindChild("PC_GOLD_COIN_SPRITE"));
        const char* cats[3][2] = { { "PC_TRADING_MENU_CLOTHING_ENTRY", "PC_RING_PIC_CLOTHING" },
                                   { "PC_TRADING_MENU_WEAPONS_ENTRY", "PC_RING_PIC_WEAPONS" },
                                   { "PC_TRADING_MENU_ITEMS_ENTRY", "PC_RING_PIC_ITEMS" } };
        for (int i = 0; i < 3; ++i)
            frame->AddPositionChild(menu->FindChild(cats[i][0])->FindChild(cats[i][1]));
        Comp* lists[3] = { menu->FindChild(cats[0][0])->FindChild("PC_TRADING_CLOTHING_LIST"),
                           menu->FindChild(cats[1][0])->FindChild("PC_TRADING_WEAPONS_LIST"),
                           menu->FindChild(cats[2][0])->FindChild("PC_TRADING_ITEMS_LIST") };
        const char* clothing[6] = { "HEAD", "HANDS", "TORSO", "SUITS", "LEGS", "FEET" };
        for (int i = 0; i < 6; ++i)
            frame->AddPositionChild(lists[0]->FindChild(std::string("PC_TRADING_MENU_") + clothing[i] + "_ENTRY")->FindChild("PC_RING_PIC_CLOTHING"));
        const char* weapons[2] = { "MELEE", "RANGED" };
        for (int i = 0; i < 2; ++i)
            frame->AddPositionChild(lists[1]->FindChild(std::string("PC_TRADING_MENU_") + weapons[i] + "_ENTRY")->FindChild(std::string("PC_RING_PIC_") + weapons[i]));
        const char* items[5] = { "TROPHIES", "POTIONS", "GIFTS", "PRODUCE", "OTHERS" };
        for (int i = 0; i < 5; ++i)
            frame->AddPositionChild(lists[2]->FindChild(std::string("PC_TRADING_MENU_") + items[i] + "_ENTRY")->FindChild(std::string("PC_RING_PIC_") + items[i]));
    }
    const char* buttons[6] = { "SCROLL_LIST_UP", "SCROLL_DESCRIPTION_UP", "SCROLL_LIST_DOWN", "SCROLL_DESCRIPTION_DOWN",
                               "PC_BACK_CLICKABLE", "PC_CLOSE_CLICKABLE" };
    for (int i = 0; i < 6; ++i) {
        Comp* b = menu->FindChild(buttons[i]);
        if (b) { gObservers.push_back(b); b->ObserveEvent(25); }
    }
}
int main()
{
    int ok = 1;
    Comp menu; menu.name = "menu";
    Comp* frame = Add(&menu, "PC_UI_INACTIVE_FRAME");
    Add(&menu, "PC_GOLD_AMOUNT_TRADE_SCREEN"); Add(&menu, "PC_GOLD_COIN_SPRITE");
    Comp* ce = Add(&menu, "PC_TRADING_MENU_CLOTHING_ENTRY"); Add(ce, "PC_RING_PIC_CLOTHING");
    Comp* we = Add(&menu, "PC_TRADING_MENU_WEAPONS_ENTRY"); Add(we, "PC_RING_PIC_WEAPONS");
    Comp* ie = Add(&menu, "PC_TRADING_MENU_ITEMS_ENTRY"); Add(ie, "PC_RING_PIC_ITEMS");
    Comp* cl = Add(ce, "PC_TRADING_CLOTHING_LIST"); Comp* wl = Add(we, "PC_TRADING_WEAPONS_LIST"); Comp* il = Add(ie, "PC_TRADING_ITEMS_LIST");
    const char* clothing[6] = { "HEAD", "HANDS", "TORSO", "SUITS", "LEGS", "FEET" };
    for (int i = 0; i < 6; ++i) Add(Add(cl, std::string("PC_TRADING_MENU_") + clothing[i] + "_ENTRY"), "PC_RING_PIC_CLOTHING");
    Add(Add(wl, "PC_TRADING_MENU_MELEE_ENTRY"), "PC_RING_PIC_MELEE"); Add(Add(wl, "PC_TRADING_MENU_RANGED_ENTRY"), "PC_RING_PIC_RANGED");
    const char* items[5] = { "TROPHIES", "POTIONS", "GIFTS", "PRODUCE", "OTHERS" };
    for (int i = 0; i < 5; ++i) Add(Add(il, std::string("PC_TRADING_MENU_") + items[i] + "_ENTRY"), std::string("PC_RING_PIC_") + items[i]);
    Add(&menu, "SCROLL_LIST_UP"); Add(&menu, "PC_CLOSE_CLICKABLE");
    AttachToFrame(&menu);
    if (frame->positionChildren.size() != 18) ok = 0;
    if (frame->positionChildren[0] != "PC_GOLD_AMOUNT_TRADE_SCREEN" || frame->positionChildren[4] != "PC_RING_PIC_ITEMS") ok = 0;
    if (frame->positionChildren[5] != "PC_RING_PIC_CLOTHING" || frame->positionChildren[11] != "PC_RING_PIC_MELEE") ok = 0;
    if (frame->positionChildren[17] != "PC_RING_PIC_OTHERS") ok = 0;
    if (gObservers.size() != 2 || gObservers[0]->name != "SCROLL_LIST_UP" || gObservers[1]->events.size() != 1 || gObservers[1]->events[0] != 25) ok = 0;
    if (!ok) { std::printf("0059bb1b_TEST FAIL\n"); return 1; }
    std::printf("0059bb1b_TEST PASS\n");
    return 0;
}