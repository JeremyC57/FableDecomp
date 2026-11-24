// Behaviour test for CTCInventoryClothing::ConstructScreenPC @ 0x005B8743 (PC clothing screen). Portable model of the row's logic in
// the row's statement order, over small UI / def / inventory mocks: mannequin set-up and the manager registrations, the item sort
// (appearance modifiers by suit part; hero suits kept when one of their owned parts is a real garment -- not one of the five
// OBJECT_HERO_NO_* placeholders -- or the suit is HERO_SUIT_NAKED), the per-part lists in head / hands / torso / suits / legs / feet
// order (name, PC guide description positioned in the PC_UI_FRAME, part id), the PC_SUB_LIST with the "CLOTHING TITLE" title and the
// title string, the worn non-removable parts map, one menu entry per part ("<name> x<count>" with the summed quantities, back action
// = close clothing, the description child + viewport fake child, the subtitle named after the caption, the change-child-state action
// with state / caption / 0 parameters), the fourth entry's PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE states dimmed to 90% red and blue and
// made the default child, the sub-list from ConstructClothingListPC (flag = worn non-removable part, never for suits) or an
// unavailable entry, and ClothingList = the returned list.
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <utility>

typedef std::vector<std::pair<long, long> > ItemVector;
enum EClothingSuitPart { CLOTHING_SUIT_NULL = 0, CLOTHING_SUIT_HEAD = 1, CLOTHING_SUIT_BODY = 2, CLOTHING_SUIT_HANDS = 3,
                         CLOTHING_SUIT_LEGS = 4, CLOTHING_SUIT_FEET = 5 };

struct State { unsigned char R, G, B; };
struct Action { int type; std::vector<std::string> params; };
struct Comp {
    std::string name, value, nameTag;
    bool unavailable;
    std::vector<Comp*> children, positioned, scrolling, nonScrolling, fake, uiParams;
    std::vector<Action> actions, backActions;
    std::map<unsigned int, State> states;
    unsigned int offsets;
    long defaultChild;
    explicit Comp(const std::string& n) : name(n), unavailable(false), offsets(0), defaultChild(-1) {}
    Comp* FindChild(const std::string& n)
    {
        for (unsigned int i = 0; i < children.size(); ++i)
            if (children[i]->name == n) return children[i];
        return 0;
    }
};
static std::vector<Comp*> gAll;
static Comp* Make(const std::string& n) { Comp* c = new Comp(n); gAll.push_back(c); return c; }

struct Manager {
    void* inventory; void* player;
    Manager() : inventory(0), player(0) {}
    Comp* CreateComponent(const std::string& n) { return Make(n); }
    Comp* CreateMenuEntryComponent(const std::string& text, const std::string& tmpl)
    {
        Comp* c = Make(tmpl);
        c->value = text;
        Comp* button = Make("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
        State a = { 205, 100, 55 }, b = { 11, 20, 255 };   // off the x87 rounding boundary (200 * 0.9f is 180 in float, 179 in extended precision)
        button->states[0] = a; button->states[1] = b;
        c->children.push_back(button);
        return c;
    }
};

struct Def {
    bool isSuit;                                   // CHeroSuitDef sub-def
    std::map<EClothingSuitPart, long> suitParts;   // CHeroSuitDef::SuitParts
    bool isModifier;                               // CAppearanceModifierDef sub-def
    EClothingSuitPart part;
    bool removeable;
};
enum { UI_ACTION_TYPE_CLOSE_CLOTHING = 58, UI_ACTION_TYPE_CHANGE_CHILD_STATE = 75 };
struct ListCall { ItemVector items; Comp* viewport; Comp* frame; EClothingSuitPart part; bool worn; };

struct Clothing {
    void* ParentThing;
    Manager* manager;
    std::map<long, Def> defs;
    std::map<std::string, long> names;             // CDefinitionManager::GetDefGlobalIndexFromName
    std::map<long, long> inventoryCount;           // GetNumberOfItemsOfTypeInInventory
    ItemVector owned;                              // GetAllItems
    std::list<long> wornModifiers;                 // GetAppearanceModifierList(APPEARANCE_CLOTHING)
    bool UseMannequin, mannequinInitialised, selectedSetUp;
    Comp* ClothingList;
    std::map<Comp*, long> ClothingItemMap;
    std::vector<ListCall> listCalls;
    std::vector<std::string> descs;

    long Index(const char* n) { std::map<std::string, long>::iterator it = names.find(n); return it == names.end() ? -1 : it->second; }
    static std::string Text(const std::string& sym) { return "T(" + sym + ")"; }
    static std::string IntToWide(long n) { char b[32]; std::sprintf(b, "%ld", n); return b; }
    Comp* CreateMenuDescPC(const std::string& sym) { Comp* c = Make("PC_DESCRIPTION"); c->value = Text(sym); descs.push_back(sym); return c; }
    void ConstructTitleString(std::string& s) { s = "TITLE"; }
    Comp* ConstructClothingListPC(ItemVector& items, Comp* viewport, Comp* frame, EClothingSuitPart part, bool worn)
    {
        ListCall c = { items, viewport, frame, part, worn };
        listCalls.push_back(c);
        if (part == CLOTHING_SUIT_FEET) return 0;  // mock: no sub-list for feet
        return Make("SUB_LIST");
    }

    Comp* ConstructScreenPC(Comp* parent)
    {
        mannequinInitialised = true;                         // InitialiseMannequin
        UseMannequin = true;
        selectedSetUp = true;                                // SetUpSelectedClothes
        manager->inventory = this;
        manager->player = ParentThing;
        Comp* frame = parent->FindChild("PC_UI_FRAME");
        ItemVector items = owned;
        ItemVector headItems, handsItems, bodyItems, legsItems, feetItems, suitItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            Def& def = defs[items[i].first];
            if (def.isSuit) {
                for (std::map<EClothingSuitPart, long>::const_iterator it = def.suitParts.begin(); it != def.suitParts.end(); ++it) {
                    if (inventoryCount[it->second] > 0) {
                        if (items[i].first == Index("HERO_SUIT_NAKED") ||
                            (it->second != Index("OBJECT_HERO_NO_HAT") && it->second != Index("OBJECT_HERO_NO_SHIRT") &&
                             it->second != Index("OBJECT_HERO_NO_GLOVES") && it->second != Index("OBJECT_HERO_NO_TROUSERS") &&
                             it->second != Index("OBJECT_HERO_NO_BOOTS"))) {
                            suitItems.push_back(items[i]);
                            break;
                        }
                    }
                }
            } else if (def.isModifier) {
                switch (def.part) {
                case CLOTHING_SUIT_HEAD:  headItems.push_back(items[i]); break;
                case CLOTHING_SUIT_BODY:  bodyItems.push_back(items[i]); break;
                case CLOTHING_SUIT_HANDS: handsItems.push_back(items[i]); break;
                case CLOTHING_SUIT_LEGS:  legsItems.push_back(items[i]); break;
                case CLOTHING_SUIT_FEET:  feetItems.push_back(items[i]); break;
                default: break;
                }
            }
        }
        std::vector<ItemVector> partLists;
        std::vector<EClothingSuitPart> parts;
        std::vector<std::string> partNames;
        std::vector<Comp*> descriptions;
        struct Sec { ItemVector* v; const char* name; const char* guide; EClothingSuitPart part; };
        Sec secs[6] = { { &headItems, "TEXT_GUI_MENU_HEAD", "TEXT_GUI_MENUGUIDE_CLOTHING_HEAD", CLOTHING_SUIT_HEAD },
                        { &handsItems, "TEXT_GUI_MENU_HANDS", "TEXT_GUI_MENUGUIDE_CLOTHING_HANDS", CLOTHING_SUIT_HANDS },
                        { &bodyItems, "TEXT_GUI_MENU_TORSO", "TEXT_GUI_MENUGUIDE_CLOTHING_TORSO", CLOTHING_SUIT_BODY },
                        { &suitItems, "TEXT_GUI_MENU_SUITS", "TEXT_GUI_MENUGUIDE_CLOTHING_SUITS", CLOTHING_SUIT_NULL },
                        { &legsItems, "TEXT_GUI_MENU_LEGS", "TEXT_GUI_MENUGUIDE_CLOTHING_LEGS", CLOTHING_SUIT_LEGS },
                        { &feetItems, "TEXT_GUI_MENU_FEET", "TEXT_GUI_MENUGUIDE_CLOTHING_FEET", CLOTHING_SUIT_FEET } };
        for (int s = 0; s < 6; ++s) {
            if (secs[s].v->empty()) continue;
            partLists.push_back(*secs[s].v);
            partNames.push_back(Text(secs[s].name));
            descriptions.push_back(CreateMenuDescPC(secs[s].guide));
            if (frame) frame->positioned.push_back(descriptions.back());
            parts.push_back(secs[s].part);
        }
        Comp* list = manager->CreateComponent("PC_SUB_LIST");
        Comp* title = manager->CreateComponent("PC_TITLE");
        title->nameTag = "CLOTHING TITLE";
        std::string titleString;
        ConstructTitleString(titleString);
        title->value = titleString;
        list->nonScrolling.push_back(title);
        if (frame) frame->positioned.push_back(title);
        unsigned long partCount = partLists.size();
        list->offsets = partCount;
        std::map<EClothingSuitPart, bool> fixedParts;
        for (std::list<long>::iterator it = wornModifiers.begin(); it != wornModifiers.end(); ++it) {
            std::map<long, Def>::iterator d = defs.find(*it);
            if (*it > 0 && d != defs.end())
                if (d->second.isModifier && !d->second.removeable) fixedParts[d->second.part] = true;
        }
        Comp* viewport = parent->FindChild("PC_DESCRIPTION_VIEWPORT");
        frame->positioned.push_back(viewport);
        ClothingItemMap.clear();
        long defaultChild = 0;
        bool dimButton = false;
        for (unsigned long i = 0; i < partCount; ++i) {
            std::string text = partNames[i];
            if (i == 3) { defaultChild = 3; dimButton = true; }
            if (!partLists[i].empty()) {
                text += " x";
                long count = 0;
                for (unsigned long j = 0; j < partLists[i].size(); ++j) count += partLists[i][j].second;
                text += IntToWide(count);
            }
            Comp* entry = manager->CreateMenuEntryComponent(text, "PC_MENU_ENTRY_BRANCH_TEMPLATE");
            if (dimButton) {
                Comp* button = entry->FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
                if (button)
                    for (std::map<unsigned int, State>::iterator it = button->states.begin(); it != button->states.end(); ++it) {
                        it->second.R = (unsigned char)(it->second.R * 0.9f);
                        it->second.B = (unsigned char)(it->second.B * 0.9f);
                    }
                dimButton = false;
            }
            Action back = { UI_ACTION_TYPE_CLOSE_CLOTHING, std::vector<std::string>() };
            entry->backActions.push_back(back);
            entry->uiParams.push_back(entry);
            entry->children.push_back(descriptions[i]);
            viewport->fake.push_back(descriptions[i]);
            Comp* subtitle = manager->CreateComponent("PC_SUBTITLE");
            subtitle->value = text;
            subtitle->nameTag = text;
            Action state = { UI_ACTION_TYPE_CHANGE_CHILD_STATE, std::vector<std::string>() };
            state.params.push_back("STATE:0"); state.params.push_back(text); state.params.push_back("SLONG:0");
            entry->actions.push_back(state);
            entry->children.push_back(subtitle);
            frame->positioned.push_back(subtitle);
            list->scrolling.push_back(entry);
            Comp* subList;
            if (parts[i] != CLOTHING_SUIT_NULL && fixedParts[parts[i]])
                subList = ConstructClothingListPC(partLists[i], viewport, frame, parts[i], true);
            else
                subList = ConstructClothingListPC(partLists[i], viewport, frame, parts[i], false);
            if (subList) entry->children.push_back(subList);
            else entry->unavailable = true;
        }
        list->defaultChild = defaultChild;
        ClothingList = list;
        return list;
    }
};

static int failures = 0;
static void Check(bool c, const char* what) { if (!c) { std::printf("FAIL: %s\n", what); ++failures; } }

static Def Modifier(EClothingSuitPart p, bool removeable) { Def d; d.isSuit = false; d.isModifier = true; d.part = p; d.removeable = removeable; return d; }
static Def Suit() { Def d; d.isSuit = true; d.isModifier = false; d.part = CLOTHING_SUIT_NULL; d.removeable = true; return d; }

int main()
{
    Manager manager;
    Clothing c;
    c.ParentThing = &manager; c.manager = &manager; c.ClothingList = 0;
    c.UseMannequin = false; c.mannequinInitialised = false; c.selectedSetUp = false;
    c.names["HERO_SUIT_NAKED"] = 900; c.names["OBJECT_HERO_NO_HAT"] = 901; c.names["OBJECT_HERO_NO_SHIRT"] = 902;
    c.names["OBJECT_HERO_NO_GLOVES"] = 903; c.names["OBJECT_HERO_NO_TROUSERS"] = 904; c.names["OBJECT_HERO_NO_BOOTS"] = 905;
    c.defs[10] = Modifier(CLOTHING_SUIT_HEAD, true);
    c.defs[11] = Modifier(CLOTHING_SUIT_HEAD, true);
    c.defs[20] = Modifier(CLOTHING_SUIT_BODY, false);   // worn below, not removable
    c.defs[30] = Modifier(CLOTHING_SUIT_HANDS, true);
    c.defs[50] = Modifier(CLOTHING_SUIT_FEET, true);
    c.defs[60] = Modifier(CLOTHING_SUIT_LEGS, true);
    Def suitA = Suit(); suitA.suitParts[CLOTHING_SUIT_HEAD] = 901; suitA.suitParts[CLOTHING_SUIT_BODY] = 20;   // owns body part 20
    Def suitB = Suit(); suitB.suitParts[CLOTHING_SUIT_HEAD] = 901; suitB.suitParts[CLOTHING_SUIT_FEET] = 905;  // placeholders only
    Def bareSuit = Suit(); bareSuit.suitParts[CLOTHING_SUIT_HEAD] = 901;                                              // HERO_SUIT_NAKED: placeholder ok
    c.defs[70] = suitA; c.defs[71] = suitB; c.defs[900] = bareSuit;
    c.defs[901] = Modifier(CLOTHING_SUIT_HEAD, true);
    c.inventoryCount[20] = 1; c.inventoryCount[901] = 1; c.inventoryCount[905] = 1;
    c.owned.push_back(std::make_pair(10L, 2L)); c.owned.push_back(std::make_pair(11L, 3L)); c.owned.push_back(std::make_pair(20L, 1L));
    c.owned.push_back(std::make_pair(30L, 4L)); c.owned.push_back(std::make_pair(50L, 1L)); c.owned.push_back(std::make_pair(70L, 1L));
    c.owned.push_back(std::make_pair(71L, 1L)); c.owned.push_back(std::make_pair(900L, 1L)); c.owned.push_back(std::make_pair(40L, 9L));
    c.wornModifiers.push_back(20); c.wornModifiers.push_back(10); c.wornModifiers.push_back(0);

    Comp* parent = Make("ROOT");
    Comp* frame = Make("PC_UI_FRAME"); Comp* viewport = Make("PC_DESCRIPTION_VIEWPORT");
    parent->children.push_back(frame); parent->children.push_back(viewport);
    Comp* list = c.ConstructScreenPC(parent);

    Check(c.mannequinInitialised && c.UseMannequin && c.selectedSetUp, "mannequin set up");
    Check(manager.inventory == &c && manager.player == &manager, "manager registrations");
    Check(c.ClothingList == list && list->name == "PC_SUB_LIST", "ClothingList is the returned list");
    // parts in order: head (10,11), hands (30), torso (20), suits (70, 900), feet (50); legs empty (60 not owned)
    Check(c.listCalls.size() == 5, "five non-empty parts");
    Check(c.descs.size() == 5 && c.descs[0] == "TEXT_GUI_MENUGUIDE_CLOTHING_HEAD" && c.descs[1] == "TEXT_GUI_MENUGUIDE_CLOTHING_HANDS" &&
          c.descs[2] == "TEXT_GUI_MENUGUIDE_CLOTHING_TORSO" && c.descs[3] == "TEXT_GUI_MENUGUIDE_CLOTHING_SUITS" &&
          c.descs[4] == "TEXT_GUI_MENUGUIDE_CLOTHING_FEET", "part order head, hands, torso, suits, feet");
    Check(c.listCalls[3].part == CLOTHING_SUIT_NULL && c.listCalls[3].items.size() == 2 && c.listCalls[3].items[0].first == 70 &&
          c.listCalls[3].items[1].first == 900, "suits: the real-garment suit and the HERO_SUIT_NAKED suit, not the placeholder suit");
    Check(list->scrolling.size() == 5 && list->offsets == 5, "one entry per part, offsets");
    Check(list->scrolling[0]->value == "T(TEXT_GUI_MENU_HEAD) x5", "head caption sums the quantities");
    Check(list->scrolling[3]->value == "T(TEXT_GUI_MENU_SUITS) x2", "suits caption");
    Check(list->nonScrolling.size() == 1 && list->nonScrolling[0]->nameTag == "CLOTHING TITLE" &&
          list->nonScrolling[0]->value == "TITLE", "title");
    Check(frame->positioned.size() == 5 + 1 + 1 + 5, "descriptions, title, viewport and subtitles positioned in the frame");
    Check(frame->positioned[6] == viewport && viewport->fake.size() == 5, "viewport positioned, descriptions are its fake children");
    Check(list->defaultChild == 3, "fourth entry is the default child");
    Comp* dimmed = list->scrolling[3]->FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
    Comp* plain = list->scrolling[2]->FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE");
    Check(dimmed->states[0].R == 184 && dimmed->states[0].G == 100 && dimmed->states[0].B == 49 &&
          dimmed->states[1].R == 9 && dimmed->states[1].B == 229, "fourth entry's button dimmed to 90% red / blue");
    Check(plain->states[0].R == 205 && plain->states[0].B == 55, "other buttons untouched");
    Check(c.listCalls[2].part == CLOTHING_SUIT_BODY && c.listCalls[2].worn, "torso flagged: non-removable worn modifier");
    Check(!c.listCalls[0].worn && !c.listCalls[1].worn && !c.listCalls[3].worn && !c.listCalls[4].worn, "other parts not flagged");
    Check(c.listCalls[0].viewport == viewport && c.listCalls[0].frame == frame, "sub-list gets viewport and frame");
    Check(list->scrolling[4]->unavailable && !list->scrolling[0]->unavailable, "no sub-list -> unavailable entry");
    Check(list->scrolling[1]->backActions.size() == 1 && list->scrolling[1]->backActions[0].type == UI_ACTION_TYPE_CLOSE_CLOTHING,
          "back action closes the clothing screen");
    Check(list->scrolling[1]->actions.size() == 1 && list->scrolling[1]->actions[0].type == UI_ACTION_TYPE_CHANGE_CHILD_STATE &&
          list->scrolling[1]->actions[0].params.size() == 3 && list->scrolling[1]->actions[0].params[1] == "T(TEXT_GUI_MENU_HANDS) x4",
          "change-child-state action with the caption");
    Check(list->scrolling[1]->uiParams.size() == 1 && list->scrolling[1]->uiParams[0] == list->scrolling[1], "entry is its own UI param");

    // fewer than four parts: default child 0, nothing dimmed
    Clothing d = c;
    d.listCalls.clear(); d.descs.clear(); d.owned.clear();
    d.owned.push_back(std::make_pair(60L, 7L));
    Comp* list2 = d.ConstructScreenPC(parent);
    Check(list2->scrolling.size() == 1 && list2->defaultChild == 0 && list2->scrolling[0]->value == "T(TEXT_GUI_MENU_LEGS) x7",
          "single legs entry, default child 0");
    Check(list2->scrolling[0]->FindChild("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE")->states[0].R == 205, "not dimmed");

    for (unsigned int i = 0; i < gAll.size(); ++i) delete gAll[i];
    if (failures) { std::printf("005b8743_TEST FAIL (%d)\n", failures); return 1; }
    std::printf("005b8743_TEST PASS\n");
    return 0;
}