// Behaviour test for CTCInventoryWeapons::ConstructScreenPC @ 0x005C9A95 (PC weapons screen). Portable model of the row's logic in
// the row's statement order, over small UI / def / inventory mocks: the manager learns the screen and the hero, the item sort
// (melee / ranged weapons by the weapon def's class, augmentations counted by quantity), the PC_SUB_LIST list with the title, the
// augmentation entry (only with augmentations), the two category menu entries ("<category> x<count>" captions, PC guide texts,
// subtitles, ring pictures), every created piece positioned in the parent's PC_UI_FRAME, the sub-lists built only for non-empty
// categories (category title, offsets, ConstructWeaponItemEntriesPC with the active weapon's damage), unavailable entries for
// empty categories, and WeaponsList = the returned list.
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <utility>

typedef std::vector<std::pair<long, long> > ItemVector;

struct Comp {
    std::string name, value, nameTag;
    bool unavailable, deleted;
    std::vector<Comp*> children, scrolling, nonScrolling, fake;
    unsigned long offsets;
    explicit Comp(const std::string& n) : name(n), unavailable(false), deleted(false), offsets(0) {}
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
    unsigned long augmentations;
    void* inventory; void* player;
    Manager() : augmentations(0), inventory(0), player(0) {}
    void SetWeaponsInventory(void* w) { inventory = w; }
    void SetPlayerThing(void* p) { player = p; }
    void SetNumberAugmentations(unsigned long n) { augmentations = n; }
    Comp* CreateComponent(const std::string& n)
    {
        Comp* c = Make(n);
        return c;
    }
    Comp* CreateMenuEntryComponent(const std::string& text, float a, float b, const std::string& tmpl)
    {
        Comp* c = Make(tmpl);
        c->value = text;
        (void)a; (void)b;
        return c;
    }
};

// defs: item def index -> kind
enum EKind { K_OTHER, K_MELEE, K_RANGED, K_AUG };
struct Defs { std::map<long, EKind> kind; std::map<long, long> shot; std::map<long, float> shotDamage; };

struct EntriesCall { ItemVector items; Comp* list; Comp* augEntry; float damage; Comp* itemTitle; };

struct Weapons {
    void* ParentThing;
    Defs* defs;
    Manager* manager;
    bool meleeAlive, rangedAlive;
    float meleeDamage;                 // CTCWeapon::GetDamage of the active melee weapon
    long rangedWeaponDef;              // the active ranged weapon's def index
    ItemVector owned;
    Comp* WeaponsList;
    Comp* PAssigned[6];
    std::vector<Comp*> AvailableWeaponsEntries;
    std::map<Comp*, long> WeaponMenuItemMap;
    std::vector<EntriesCall> calls;
    std::vector<ItemVector> augCalls;

    std::string Text(const std::string& sym) const { return "T(" + sym + ")"; }
    static std::string IntToWide(unsigned long n) { char b[16]; std::sprintf(b, "%lu", n); return b; }
    Comp* ConstructAugEntry(bool, ItemVector& augs, Comp* viewport) { augCalls.push_back(augs); (void)viewport; return Make("AUG_ENTRY"); }
    void ConstructWeaponItemEntries(ItemVector& items, Comp* list, Comp*& augEntry, Comp*, Manager*, Comp*, float damage, Comp*& title)
    {
        EntriesCall c = { items, list, augEntry, damage, title };
        calls.push_back(c);
    }

    Comp* ConstructScreenPC(Comp* parent)
    {
        manager->SetWeaponsInventory(this);
        manager->SetPlayerThing(ParentThing);
        for (int i = 0; i < 6; ++i) PAssigned[i] = 0;
        AvailableWeaponsEntries.clear();
        ItemVector items, meleeItems, rangedItems, augmentations;
        items = owned;
        long augmentationCount = 0;
        for (unsigned long i = 0; i < items.size(); ++i) {
            EKind k = defs->kind[items[i].first];
            if (k == K_MELEE || k == K_RANGED) {
                if (k == K_MELEE) meleeItems.push_back(items[i]);
                else rangedItems.push_back(items[i]);
            } else if (k == K_AUG) {
                augmentations.push_back(items[i]);
                augmentationCount += items[i].second;
            }
        }
        Comp* frame = parent->FindChild("PC_UI_FRAME");
        manager->SetNumberAugmentations(augmentationCount);
        Comp* augmentationEntry = 0;
        Comp* list = manager->CreateComponent("PC_SUB_LIST");
        Comp* viewport = parent->FindChild("PC_DESCRIPTION_VIEWPORT");
        frame->children.push_back(viewport);
        if (augmentations.size() > 0)
            augmentationEntry = ConstructAugEntry(true, augmentations, viewport);
        Comp* title = manager->CreateComponent("PC_TITLE_WEAPONS");
        list->nonScrolling.push_back(title);
        frame->children.push_back(title);
        WeaponMenuItemMap.clear();

        std::string meleeText = Text("CAT_MELEE");
        if (!meleeItems.empty()) { meleeText += " x"; meleeText += IntToWide(meleeItems.size()); }
        Comp* meleeEntry = manager->CreateMenuEntryComponent(meleeText, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE");
        list->scrolling.push_back(meleeEntry);
        Comp* description = manager->CreateComponent("PC_DESCRIPTION");
        description->value = Text("TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_PC");
        meleeEntry->children.push_back(description);
        viewport->fake.push_back(description);
        frame->children.push_back(description);
        Comp* meleeSubtitle = manager->CreateComponent("PC_SUBTITLE");
        meleeSubtitle->value = meleeText;
        meleeEntry->children.push_back(meleeSubtitle);
        frame->children.push_back(meleeSubtitle);

        std::string rangedText = Text("CAT_RANGED");
        if (!rangedItems.empty()) { rangedText += " x"; rangedText += IntToWide(rangedItems.size()); }
        Comp* rangedEntry = manager->CreateMenuEntryComponent(rangedText, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE");
        list->scrolling.push_back(rangedEntry);
        description = manager->CreateComponent("PC_DESCRIPTION");
        description->value = Text("TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_PC");
        rangedEntry->children.push_back(description);
        viewport->fake.push_back(description);
        frame->children.push_back(description);
        Comp* rangedSubtitle = manager->CreateComponent("PC_SUBTITLE");
        rangedSubtitle->value = rangedText;
        rangedEntry->children.push_back(rangedSubtitle);
        frame->children.push_back(rangedSubtitle);

        Comp* meleeTitle = manager->CreateComponent("PC_TITLE");
        meleeTitle->value = "TEXT_GUI_MENU_MELEE";
        frame->children.push_back(meleeTitle);
        Comp* meleeItemTitle = manager->CreateComponent("PC_TITLE");
        meleeItemTitle->value = "TEXT_GUI_MENU_MELEE";
        frame->children.push_back(meleeItemTitle);
        Comp* meleeRing = manager->CreateComponent("PC_RING_PIC_MELEE");
        meleeEntry->children.push_back(meleeRing);
        frame->children.push_back(meleeRing);
        Comp* rangedRing = manager->CreateComponent("PC_RING_PIC_RANGED");
        rangedEntry->children.push_back(rangedRing);
        frame->children.push_back(rangedRing);

        if (meleeItems.empty()) {
            meleeEntry->unavailable = true;
        } else {
            Comp* subList = manager->CreateComponent("PC_SUB_LIST");
            meleeEntry->children.push_back(subList);
            subList->nonScrolling.push_back(meleeTitle);
            subList->offsets = meleeItems.size();
            float damage = 0.0f;
            if (meleeAlive) damage = meleeDamage;
            ConstructWeaponItemEntries(meleeItems, subList, augmentationEntry, parent, manager, viewport, damage, meleeItemTitle);
        }

        Comp* rangedTitle = manager->CreateComponent("PC_TITLE");
        rangedTitle->value = "TEXT_GUI_MENU_RANGED";
        frame->children.push_back(rangedTitle);
        Comp* rangedItemTitle = manager->CreateComponent("PC_TITLE");
        rangedItemTitle->value = "TEXT_GUI_MENU_RANGED";
        frame->children.push_back(rangedItemTitle);
        if (rangedItems.empty()) {
            rangedEntry->unavailable = true;
        } else {
            Comp* subList = manager->CreateComponent("PC_SUB_LIST");
            rangedEntry->children.push_back(subList);
            subList->nonScrolling.push_back(rangedTitle);
            subList->offsets = rangedItems.size();
            float damage = 0.0f;
            if (rangedAlive) damage = defs->shotDamage[defs->shot[rangedWeaponDef]];
            ConstructWeaponItemEntries(rangedItems, subList, augmentationEntry, parent, manager, viewport, damage, rangedItemTitle);
        }
        WeaponsList = list;
        return list;
    }
};

static int failures = 0;
static void Check(bool c, const char* what) { if (!c) { std::printf("FAIL: %s\n", what); ++failures; } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    Defs defs;
    defs.kind[10] = K_MELEE; defs.kind[11] = K_MELEE; defs.kind[20] = K_RANGED; defs.kind[30] = K_AUG; defs.kind[31] = K_AUG;
    defs.kind[40] = K_OTHER; defs.shot[20] = 200; defs.shotDamage[200] = 17.5f;
    Manager manager;
    Comp parent("parent");
    Comp* frame = Make("PC_UI_FRAME"); Comp* viewport = Make("PC_DESCRIPTION_VIEWPORT");
    parent.children.push_back(frame); parent.children.push_back(viewport);
    int hero = 0;

    // 1: a mixed inventory
    Weapons w; w.defs = &defs; w.manager = &manager; w.meleeAlive = true; w.rangedAlive = true; w.meleeDamage = 12.25f;
    w.rangedWeaponDef = 20; w.WeaponsList = 0; w.ParentThing = &hero;
    w.owned.push_back(std::make_pair(10L, 1L)); w.owned.push_back(std::make_pair(30L, 3L)); w.owned.push_back(std::make_pair(20L, 1L));
    w.owned.push_back(std::make_pair(40L, 5L)); w.owned.push_back(std::make_pair(11L, 1L)); w.owned.push_back(std::make_pair(31L, 2L));
    Comp* list = w.ConstructScreenPC(&parent);
    Check(list && list->name == "PC_SUB_LIST" && w.WeaponsList == list, "returns the list and stores WeaponsList");
    Check(manager.inventory == &w && manager.player == &hero, "manager knows the screen and the hero");
    Check(manager.augmentations == 5, "augmentation count sums the quantities");
    Check(w.augCalls.size() == 1 && w.augCalls[0].size() == 2, "one augmentation entry over the two augmentation items");
    Check(list->nonScrolling.size() == 1 && list->nonScrolling[0]->name == "PC_TITLE_WEAPONS", "list title");
    Check(list->scrolling.size() == 2, "two category entries");
    Check(list->scrolling[0]->value == "T(CAT_MELEE) x2" && list->scrolling[1]->value == "T(CAT_RANGED) x1", "category captions with counts");
    Check(!list->scrolling[0]->unavailable && !list->scrolling[1]->unavailable, "both categories available");
    Check(w.calls.size() == 2, "two item-entry builders");
    Check(w.calls[0].items.size() == 2 && w.calls[0].items[0].first == 10 && w.calls[0].items[1].first == 11, "melee items in order");
    Check(Near(w.calls[0].damage, 12.25f) && Near(w.calls[1].damage, 17.5f), "active weapon damages");
    Check(w.calls[0].augEntry && w.calls[0].augEntry == w.calls[1].augEntry, "both builders get the augmentation entry");
    Check(w.calls[0].list->offsets == 2 && w.calls[0].list->nonScrolling.size() == 1, "melee sub-list offsets and title");
    Check(w.calls[1].list->offsets == 1 && w.calls[1].list->nonScrolling[0]->value == "TEXT_GUI_MENU_RANGED", "ranged sub-list title");
    Check(frame->children.size() == 12 && frame->children[0] == viewport, "frame positions the viewport and the 11 pieces");
    Check(viewport->fake.size() == 2 && viewport->fake[0]->value == "T(TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_PC)", "PC guide texts");

    // 2: nothing but junk: both categories unavailable, no builders, no augmentation entry
    frame->children.clear(); viewport->fake.clear();
    Weapons e; e.defs = &defs; e.manager = &manager; e.meleeAlive = false; e.rangedAlive = false; e.meleeDamage = 3.0f;
    e.rangedWeaponDef = 20; e.WeaponsList = 0; e.ParentThing = &hero;
    e.owned.push_back(std::make_pair(40L, 2L));
    Comp* l2 = e.ConstructScreenPC(&parent);
    Check(e.WeaponsList == l2 && manager.augmentations == 0 && e.augCalls.empty() && e.calls.empty(), "empty screen");
    Check(l2->scrolling[0]->unavailable && l2->scrolling[1]->unavailable, "both categories unavailable");
    Check(l2->scrolling[0]->value == "T(CAT_MELEE)", "no count without items");

    // 3: dead active weapons give zero damage
    Weapons d = w; d.calls.clear(); d.augCalls.clear(); d.meleeAlive = false; d.rangedAlive = false;
    d.ConstructScreenPC(&parent);
    Check(d.calls.size() == 2 && Near(d.calls[0].damage, 0.0f) && Near(d.calls[1].damage, 0.0f), "dead weapons: zero damage");

    for (unsigned int i = 0; i < gAll.size(); ++i) delete gAll[i];
    if (failures == 0) std::printf("005c9a95_TEST PASS\n");
    return failures != 0;
}