// Behaviour test for NInventory::CTCInventoryBase::CreateInventoryObject (0x005BE8F3): a copy of the function's logic over a small model.
#include <cstdio>
#include <vector>
// STLport min(a, b): b < a ? b : a (the test compiler's own <algorithm> spells it differently)
template <class T> inline const T& stlp_min(const T& a, const T& b) { return b < a ? b : a; }

struct CPerItemData { std::vector<long> Augmentations; long NumberOfKills; long TimeWielded; long BestTrophyWitnessesAheadToDate; };
class CTCWeapon { public: long NumThingsKilled; long TimeWielded; };
class CTCTrophy { public: long BestWitnessesAheadToDate; };
struct CInventory;
class CTCInventoryItem { public: CInventory* PInventory; void ConnectToInventory(CInventory& inv) { PInventory = &inv; } };
class CTCObjectAugmentations { public:
    long MaxSlots; std::vector<long> Slots;
    long GetMaximumNumberOfSlots() const { return MaxSlots; }
    long GetNumberOfSlots() const { return (long)Slots.size(); }
    void AddNewSlot() { Slots.push_back(-1); }
    void AttachAugmentationToSlot(long aug, long slot) { Slots[slot] = aug; }
    void RemoveAugmentationFromSlot(long slot) { Slots[slot] = 0; }
};
class CThing { public:
    bool Dead; long DefIndex; long Player;
    CTCWeapon* Weapon; CTCObjectAugmentations* Augs; CTCTrophy* Trophy; CTCInventoryItem* Item;
    bool GetTC(CTCWeapon** p) { if (Weapon) { *p = Weapon; return true; } return false; }
    bool GetTC(CTCObjectAugmentations** p) { if (Augs) { *p = Augs; return true; } return false; }
    bool GetTC(CTCTrophy** p) { if (Trophy) { *p = Trophy; return true; } return false; }
    bool GetTC(CTCInventoryItem** p) { if (Item) { *p = Item; return true; } return false; }
};
static bool IS_THING_ALIVE(CThing* const& thing) { if (thing != 0 && !thing->Dead) return true; return false; }

static CThing* g_next;          // what Create hands out
static int g_creates;
static CThing* Create(long defIndex, long player) { ++g_creates; g_next->DefIndex = defIndex; g_next->Player = player; return g_next; }

struct CInventory {
    long Owned; const CPerItemData* Data; long Player;
    long GetNumberOfItemsOfTypeInInventory(long) const { return Owned; }
    const CPerItemData* GetPerItemDataForItem(long) const { return Data; }
    CThing* CreateInventoryObject(long itemDefIndex)
    {
        CThing* thing = 0;
        if (GetNumberOfItemsOfTypeInInventory(itemDefIndex) > 0) {
            thing = Create(itemDefIndex, Player);
            if (IS_THING_ALIVE(thing)) {
                const CPerItemData* data = GetPerItemDataForItem(itemDefIndex);
                if (data) {
                    CTCWeapon* weapon;
                    if (thing->GetTC(&weapon)) {
                        weapon->NumThingsKilled = data->NumberOfKills;
                        weapon->TimeWielded = data->TimeWielded;
                    }
                    CTCObjectAugmentations* augmentations;
                    if (thing->GetTC(&augmentations)) {
                        long max = augmentations->GetMaximumNumberOfSlots();
                        long count = stlp_min(max, (long)data->Augmentations.size());
                        while (augmentations->GetNumberOfSlots() < count)
                            augmentations->AddNewSlot();
                        for (long i = 0; i < count; ++i) {
                            if (data->Augmentations[i] > 0)
                                augmentations->AttachAugmentationToSlot(data->Augmentations[i], i);
                            else
                                augmentations->RemoveAugmentationFromSlot(i);
                        }
                    }
                    CTCTrophy* trophy;
                    if (thing->GetTC(&trophy))
                        trophy->BestWitnessesAheadToDate = data->BestTrophyWitnessesAheadToDate;
                }
                CTCInventoryItem* item = 0;
                thing->GetTC(&item);
                item->ConnectToInventory(*this);
            }
        }
        return thing;
    }
};

static int Fail(const char* why) { std::printf("005be8f3_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    CTCWeapon weapon = { -1, -1 };
    CTCTrophy trophy = { -1 };
    CTCInventoryItem item = { 0 };
    CTCObjectAugmentations augs; augs.MaxSlots = 3; augs.Slots.push_back(-1);
    CThing thing = { false, 0, 0, &weapon, &augs, &trophy, &item };
    CPerItemData data; data.NumberOfKills = 17; data.TimeWielded = 900; data.BestTrophyWitnessesAheadToDate = 6;
    data.Augmentations.push_back(41); data.Augmentations.push_back(0); data.Augmentations.push_back(43); data.Augmentations.push_back(44);
    CInventory inv = { 0, &data, 2 };
    g_next = &thing;

    // nothing owned: no object is created
    if (inv.CreateInventoryObject(77) != 0 || g_creates != 0) return Fail("not owned");

    // owned, alive, with data: everything restored; slots = min(max 3, 4 stored) = 3
    inv.Owned = 1;
    if (inv.CreateInventoryObject(77) != &thing || g_creates != 1) return Fail("create");
    if (thing.DefIndex != 77 || thing.Player != 2) return Fail("create args");
    if (weapon.NumThingsKilled != 17 || weapon.TimeWielded != 900) return Fail("weapon");
    if (augs.Slots.size() != 3 || augs.Slots[0] != 41 || augs.Slots[1] != 0 || augs.Slots[2] != 43) return Fail("augmentations");
    if (trophy.BestWitnessesAheadToDate != 6) return Fail("trophy");
    if (item.PInventory != &inv) return Fail("connect");

    // fewer stored augmentations than the maximum: only that many slots are touched, none added beyond the existing ones
    CTCObjectAugmentations augs2; augs2.MaxSlots = 5; augs2.Slots.push_back(9); augs2.Slots.push_back(9); augs2.Slots.push_back(9);
    CPerItemData data2; data2.NumberOfKills = 1; data2.TimeWielded = 2; data2.BestTrophyWitnessesAheadToDate = 3;
    data2.Augmentations.push_back(0); data2.Augmentations.push_back(52);
    CTCInventoryItem item2 = { 0 };
    CThing thing2 = { false, 0, 0, 0, &augs2, 0, &item2 };
    inv.Data = &data2; g_next = &thing2;
    if (inv.CreateInventoryObject(5) != &thing2) return Fail("second");
    if (augs2.Slots.size() != 3 || augs2.Slots[0] != 0 || augs2.Slots[1] != 52 || augs2.Slots[2] != 9) return Fail("partial slots");
    if (item2.PInventory != &inv) return Fail("connect 2");

    // no per-item data: only the inventory connection happens
    CTCWeapon weapon3 = { -5, -6 }; CTCInventoryItem item3 = { 0 };
    CThing thing3 = { false, 0, 0, &weapon3, 0, 0, &item3 };
    inv.Data = 0; g_next = &thing3;
    if (inv.CreateInventoryObject(6) != &thing3) return Fail("third");
    if (weapon3.NumThingsKilled != -5 || weapon3.TimeWielded != -6 || item3.PInventory != &inv) return Fail("no data");

    // dead thing: returned untouched
    CTCInventoryItem item4 = { 0 };
    CThing thing4 = { true, 0, 0, 0, 0, 0, &item4 };
    inv.Data = &data; g_next = &thing4;
    if (inv.CreateInventoryObject(8) != &thing4 || item4.PInventory != 0) return Fail("dead");

    std::printf("005be8f3_TEST PASS\n");
    return 0;
}