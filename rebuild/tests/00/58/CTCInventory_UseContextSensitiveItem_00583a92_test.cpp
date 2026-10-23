// 00583a92 behaviour test: CTCInventory::UseContextSensitiveItem (match.cpp body, host-modelled)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct CDefClassBase {
    long RefCount; int destroyed;
    CDefClassBase() : RefCount(1), destroyed(0) {}
    virtual ~CDefClassBase() {}
    void Destroy() { ++destroyed; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() { Object = 0; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CContextSensitiveItemDef : CDefClassBase { long ItemType; long Slot; };
static CContextSensitiveItemDef* g_sub = 0;
struct CThingDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CContextSensitiveItemDef>* out)
    { if (g_sub) ++g_sub->RefCount; out->Object = g_sub; return g_sub != 0; }
};
static CThingDef* g_def = 0;
struct CDefinitionManager {
    bool GetDef(long, CDefPointer<CThingDef>* out)
    { if (g_def) ++g_def->RefCount; out->Object = g_def; return g_def != 0; }
};
static CDefinitionManager g_mgr;

struct CThing { bool alive; unsigned short DefIndex; };
static bool IS_THING_ALIVE(CThing* const& thing) { return thing != 0 && thing->alive; }
struct CTCCarrying { CThing* held; CThing* GetThingInPrimarySlot() { return held; } };

template <class T> struct CRetailVector {
    T* _M_start;
    T* begin() const { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

struct CTCInventory {
    CTCCarrying* carryingTC;
    CRetailVector<long> QuickAccessItems, SlotItems, PreviousSlotItems;
    int usedThing, usedDef; long usedDefIndex; CThing* usedThingPtr;
    CDefinitionManager* GetDefinitions() const { return &g_mgr; }
    void UseItemDef(long defIndex, bool& used) { ++usedDef; usedDefIndex = defIndex; used = true; }
    void UseItemThing(CThing* thing, bool& used) { ++usedThing; usedThingPtr = thing; used = true; }
    void UseContextSensitiveItem(long quickSlot)
    {
        CTCCarrying* carrying = carryingTC;
        if (QuickAccessItems[quickSlot] > 0) {
            CDefPointer<CThingDef> thingDef;
            if (GetDefinitions()->GetDef(QuickAccessItems[quickSlot], &thingDef)) {
                CDefPointer<CContextSensitiveItemDef> itemDef;
                if (thingDef->GetSubDef(&itemDef) && itemDef->ItemType == 6 && itemDef->Slot >= 0 && itemDef->Slot < 6) {
                    PreviousSlotItems[itemDef->Slot] = SlotItems[itemDef->Slot];
                    SlotItems[itemDef->Slot] = QuickAccessItems[quickSlot];
                }
            }
            CThing* held = carrying->GetThingInPrimarySlot();
            if (IS_THING_ALIVE(held) && held->DefIndex == QuickAccessItems[quickSlot]) {
                bool used;
                UseItemThing(held, used);
            } else {
                bool used;
                UseItemDef(QuickAccessItems[quickSlot], used);
            }
        }
    }
};

int main()
{
    long qa[4] = { 0, 17, -3, 9 }, slots[6] = { 1, 2, 3, 4, 5, 6 }, prev[6] = { 0, 0, 0, 0, 0, 0 };
    CThing held = { true, 17 };
    CTCCarrying carrying = { &held };
    CTCInventory inv;
    inv.carryingTC = &carrying;
    inv.QuickAccessItems._M_start = qa; inv.SlotItems._M_start = slots; inv.PreviousSlotItems._M_start = prev;
    inv.usedThing = inv.usedDef = 0; inv.usedDefIndex = 0; inv.usedThingPtr = 0;
    CThingDef def; CContextSensitiveItemDef sub; sub.ItemType = 6; sub.Slot = 2;
    g_def = &def; g_sub = &sub;

    // slot 1: def 17, type-6 sub-def slot 2 -> slot items updated; held thing alive with matching def -> use the thing
    inv.UseContextSensitiveItem(1);
    CHECK(prev[2] == 3 && slots[2] == 17);
    CHECK(inv.usedThing == 1 && inv.usedThingPtr == &held && inv.usedDef == 0);
    CHECK(def.RefCount == 1 && sub.RefCount == 1 && def.destroyed == 0 && sub.destroyed == 0);

    // non-positive quick item: nothing happens
    inv.UseContextSensitiveItem(0);
    inv.UseContextSensitiveItem(2);
    CHECK(inv.usedThing == 1 && inv.usedDef == 0);

    // def 9: held thing has another def -> use the def; slot 7 out of range leaves the slot items alone
    sub.Slot = 7;
    inv.UseContextSensitiveItem(3);
    CHECK(inv.usedDef == 1 && inv.usedDefIndex == 9 && slots[2] == 17 && prev[2] == 3);

    // wrong item type, dead held thing -> use the def
    sub.Slot = 1; sub.ItemType = 5; held.alive = false;
    inv.UseContextSensitiveItem(1);
    CHECK(inv.usedDef == 2 && inv.usedDefIndex == 17 && slots[1] == 2 && prev[1] == 0);

    // no def found: still uses by def; no held thing
    g_def = 0; carrying.held = 0;
    inv.UseContextSensitiveItem(3);
    CHECK(inv.usedDef == 3 && inv.usedDefIndex == 9 && inv.usedThing == 1);

    // a def whose last reference is dropped here is destroyed
    CThingDef def2; def2.RefCount = 0; g_def = &def2; g_sub = 0;
    inv.UseContextSensitiveItem(1);
    CHECK(def2.destroyed == 1 && inv.usedDef == 4);

    if (!g_fail) printf("00583a92_TEST PASS\n");
    return g_fail;
}