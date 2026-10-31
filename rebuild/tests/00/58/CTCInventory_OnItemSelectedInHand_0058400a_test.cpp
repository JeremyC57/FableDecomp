// Behaviour test for CTCInventory::OnItemSelectedInHand (0x0058400A).
#include <cstdio>

struct Creature { unsigned char ThingType; bool Busy; int SheatheActions; };
struct Use {
    bool UsableWhileBusy; int Uses; Creature* LastUser;
    bool IsUsableWhileThingPerformingAction() const { return UsableWhileBusy; }
    void OnActionUseInHand(Creature& c) { ++Uses; LastUser = &c; }
};
struct Item { Use* Component; };   // null = no interface 0x0E

struct Inventory {
    Creature* ParentThing;
    void OnItemSelectedInHand(Item& item, bool& handled)
    {
        (void)handled;
        if (item.Component) {
            Use* use = item.Component;
            if (!ParentThing->Busy || use->IsUsableWhileThingPerformingAction())
                use->OnActionUseInHand(*ParentThing);
        } else {
            Creature* creature = ParentThing;
            if (creature->ThingType == 2)
                ++creature->SheatheActions;
        }
    }
};

#define FAIL() do { std::printf("0058400a_TEST FAIL\n"); return 1; } while (0)
int main()
{
    Creature owner = {2, false, 0};
    Inventory inv = {&owner};
    Use use = {false, 0, 0};
    Item withUse = {&use}, plain = {0};
    bool handled = false;
    inv.OnItemSelectedInHand(withUse, handled);
    if (use.Uses != 1 || use.LastUser != &owner) FAIL();
    owner.Busy = true;
    inv.OnItemSelectedInHand(withUse, handled);
    if (use.Uses != 1) FAIL();
    use.UsableWhileBusy = true;
    inv.OnItemSelectedInHand(withUse, handled);
    if (use.Uses != 2) FAIL();
    inv.OnItemSelectedInHand(plain, handled);
    if (owner.SheatheActions != 1) FAIL();
    owner.ThingType = 1;
    inv.OnItemSelectedInHand(plain, handled);
    if (owner.SheatheActions != 1 || handled) FAIL();
    std::printf("0058400a_TEST PASS\n");
    return 0;
}