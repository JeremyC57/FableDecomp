// Behaviour test for CTCInventory::AssignThingPC (0x00584B86): a copy of the row's logic over a small model of the UI
// component, action and def lookups. Checks the old-contents name choice, the deletion mark, the icon graphic, and the
// action types / parameters for a plain thing and for a spell (a thing whose def carries an ability sub-def).
#include <cstdio>
#include <cstring>
#include <vector>

struct Action {
    long type;
    std::vector<long> params;
    std::vector<const void*> targets;
};

struct Component {
    const char* name;
    bool deleted;
    unsigned long graphic;
    bool observingMouse;
    std::vector<Action> droppedNowhere;
    std::vector<Action> dropped;
    Component* contents;
    Component* child;           // the icon's sprite
    Component(const char* n) : name(n), deleted(false), graphic(0), observingMouse(false), contents(0), child(0) {}
    Component* FindChild(const char* n)
    {
        if (contents && std::strcmp(contents->name, n) == 0) return contents;
        if (child && std::strcmp(child->name, n) == 0) return child;
        return 0;
    }
};

struct Inventory {
    long quickAccess[8];
    Component* created;
    void SetThingAsQuickAccessItem(long thing, long slot, bool) { quickAccess[slot] = thing; }
    // def lookup model: things >= 100 are spells whose ability is thing - 100
    bool GetAbility(long thing, long* ability) { if (thing >= 100) { *ability = thing - 100; return true; } return false; }
    unsigned long GetEngineGraphic(long thing) { return (unsigned long)(thing * 3 + 1); }

    void AssignThingPC(long thing, unsigned long slot, Component* container)
    {
        long previous = quickAccess[slot];
        SetThingAsQuickAccessItem(thing, slot, true);
        Component* old;
        if (previous == 0)
            old = container->FindChild("PC_EXPRESSION_ITEM_INSIDE_CONTAINER");
        else
            old = container->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER");
        if (old)
            old->deleted = true;
        Component* icon = new Component("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER");
        icon->child = new Component("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
        created = icon;
        Component* sprite = icon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
        if (sprite)
            sprite->graphic = GetEngineGraphic(thing);
        Action action;
        action.type = 252;
        action.targets.push_back(container);
        action.params.push_back((long)slot);
        icon->droppedNowhere.push_back(action);
        bool isSpell = false;
        long ability = 0;
        Action rearm;
        if (GetAbility(thing, &ability)) {
            isSpell = true;
            rearm.type = 246;
        } else {
            rearm.type = 253;
        }
        rearm.params.push_back(0);
        icon->droppedNowhere.push_back(rearm);
        icon->observingMouse = true;
        Action swap;
        if (!isSpell) {
            swap.type = 327;
            swap.params.push_back(thing);
            swap.params.push_back((long)slot);
        } else {
            swap.type = 329;
            swap.params.push_back(ability);
            swap.params.push_back((long)slot);
        }
        swap.targets.push_back(container);
        icon->dropped.push_back(swap);
        container->contents = icon;
    }
};

static int fail() { std::printf("00584b86_TEST FAIL\n"); return 1; }

int main()
{
    Inventory inv;
    for (int i = 0; i < 8; ++i) inv.quickAccess[i] = 0;
    Component container("SLOT");
    Component empty("PC_EXPRESSION_ITEM_INSIDE_CONTAINER");
    container.contents = &empty;

    // empty slot 2 gets thing 7: the empty item is marked deleted
    inv.AssignThingPC(7, 2, &container);
    Component* first = inv.created;
    if (inv.quickAccess[2] != 7 || !empty.deleted || container.contents != first) return fail();
    if (first->child->graphic != 22 || !first->observingMouse) return fail();
    if (first->droppedNowhere.size() != 2 || first->droppedNowhere[0].type != 252 || first->droppedNowhere[0].params[0] != 2) return fail();
    if (first->droppedNowhere[0].targets[0] != &container || first->droppedNowhere[1].type != 253) return fail();
    if (first->dropped.size() != 1 || first->dropped[0].type != 327 || first->dropped[0].params[0] != 7 || first->dropped[0].params[1] != 2)
        return fail();

    // occupied slot 2 gets spell 105: the previous assignment icon is marked deleted
    inv.AssignThingPC(105, 2, &container);
    Component* second = inv.created;
    if (!first->deleted || container.contents != second || inv.quickAccess[2] != 105) return fail();
    if (second->droppedNowhere[1].type != 246 || second->dropped[0].type != 329 || second->dropped[0].params[0] != 5
        || second->dropped[0].params[1] != 2) return fail();

    delete first->child; delete first; delete second->child; delete second;
    std::printf("00584b86_TEST PASS\n");
    return 0;
}