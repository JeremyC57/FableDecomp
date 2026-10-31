// Test for 0x00849700 CCreatureAction_StealThingFromShop::PerformAction() -- the steal flow over mock engine objects
#include <stdio.h>
#include <string.h>
struct Log { int created; int added; int messages; int noises; const char* subtitle; float subtitleTime; long msgType; long msgDelay; };
static Log g;
class CThing;
struct CTCInventoryBase { int room; bool IsSpaceInInventoryForItem(long) const { return room != 0; } };
class CTCInventoryItem { public:
    int maxItems;
    int GetMaxNumberOfItems() const { return maxItems; }
    void AddToInventory(CTCInventoryBase&, bool, bool, long, bool, bool) { ++g.added; }
};
class CTCOwnedEntity { public: CThing* POwner; };
class CThing { public:
    bool Dead; long DefGlobalIndex; CTCInventoryItem item; CTCOwnedEntity owned; CTCInventoryBase inventory;
    void GetInterface(CTCInventoryItem** out) { *out = &item; }
    void GetInterface(CTCOwnedEntity** out) { *out = &owned; }
    CTCInventoryBase* GetCheckedTC() { return &inventory; }
    void MakeNoise(const char* name) { if (!strcmp(name, "SIMPLE_NOISE")) ++g.noises; }
};
static CThing g_copy;
static CThing* CreateThingAt(const CThing* item, const CThing*, const CThing*, const char*)
{
    ++g.created; g_copy.DefGlobalIndex = item->DefGlobalIndex; return &g_copy;
}
static void AddMessage(long type, long delay, CThing*, CThing*) { ++g.messages; g.msgType = type; g.msgDelay = delay; }
static void SetText(const char* text, float time) { g.subtitle = text; g.subtitleTime = time; }
struct CCreatureAction_StealThingFromShop {
    CThing* PThingToSteal; CThing* PCreature;
    void PerformAction();
};
void CCreatureAction_StealThingFromShop::PerformAction()
{
    if (PThingToSteal && !PThingToSteal->Dead) {
        CTCInventoryItem* inventoryItem;
        PThingToSteal->GetInterface(&inventoryItem);
        CTCInventoryBase* inventory = PCreature->GetCheckedTC();
        if (!inventory->IsSpaceInInventoryForItem(PThingToSteal->DefGlobalIndex)) {
            const char* message;
            if (inventoryItem->GetMaxNumberOfItems() == 1)
                message = "You already have this item!";
            else
                message = "You don't have room in your inventory for this item!";
            SetText(message, 3.0f);
            return;
        }
        CThing* copy = CreateThingAt(PThingToSteal, PCreature, PCreature, "");
        CTCInventoryItem* copyItem;
        copy->GetInterface(&copyItem);
        copyItem->AddToInventory(*inventory, false, true, 0, false, false);
        CTCOwnedEntity* owned;
        PThingToSteal->GetInterface(&owned);
        AddMessage(0x1c, 0x32, PCreature, owned->POwner);
        PCreature->MakeNoise("SIMPLE_NOISE");
    }
}
int main()
{
    int ok = 1;
    CThing item, creature, owner;
    memset(&item, 0, sizeof item); memset(&creature, 0, sizeof creature); memset(&owner, 0, sizeof owner);
    item.DefGlobalIndex = 77; item.owned.POwner = &owner; item.item.maxItems = 1;
    CCreatureAction_StealThingFromShop a; a.PThingToSteal = &item; a.PCreature = &creature;
    memset(&g, 0, sizeof g);
    a.PerformAction();                                          // no room, single-holding item
    ok &= g.subtitle && !strcmp(g.subtitle, "You already have this item!") && g.subtitleTime == 3.0f && g.created == 0;
    item.item.maxItems = 5; memset(&g, 0, sizeof g);
    a.PerformAction();
    ok &= g.subtitle && !strcmp(g.subtitle, "You don't have room in your inventory for this item!") && g.added == 0;
    creature.inventory.room = 1; memset(&g, 0, sizeof g);
    a.PerformAction();                                          // steal
    ok &= g.created == 1 && g_copy.DefGlobalIndex == 77 && g.added == 1 && g.messages == 1 && g.msgType == 0x1c
       && g.msgDelay == 0x32 && g.noises == 1 && g.subtitle == 0;
    item.Dead = true; memset(&g, 0, sizeof g);
    a.PerformAction();                                          // dead item: nothing
    ok &= g.created == 0 && g.messages == 0;
    a.PThingToSteal = 0;
    a.PerformAction();
    ok &= g.created == 0;
    printf(ok ? "00849700_TEST PASS\n" : "00849700_TEST FAIL\n");
    return ok ? 0 : 1;
}