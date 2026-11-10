// Behaviour test of CTCInventoryWeapons::GetFreeAugmentationSlotForWeapon (0x005C4671): the active melee / ranged weapon of the
// asked def answers directly; otherwise a temporary inventory object is created, asked, and returned to the inventory.
#include <cstdio>
class CTCObjectAugmentations { public: long freeSlot; long GetFreeSlot() const { return freeSlot; } };
class CTCInventoryItem { public: int returned; void ReturnToInventory() { ++returned; } };
class CThing {
public:
    unsigned long defIndex;
    CTCObjectAugmentations* augmentations;        // 0 when the thing has none
    CTCInventoryItem item;
    unsigned long GetDefGlobalIndex() const { return defIndex; }
    bool PeekTC(const CTCObjectAugmentations** out) const { if (!augmentations) return false; *out = augmentations; return true; }
    bool GetTC(CTCInventoryItem** out) { *out = &item; return true; }
};
struct CIntelligentPointer { CThing* item; CThing* Get() const { return item; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.item != 0; }
class CTCInventoryWeapons {
public:
    CIntelligentPointer PActiveMeleeWeapon, PActiveRangedWeapon;
    CThing* created; int creations;
    CThing* CreateInventoryObject(long itemDefIndex) { ++creations; created->defIndex = itemDefIndex; return created; }
    long GetFreeAugmentationSlotForWeapon(long weaponDefIndex)
    {
        long freeSlot = -1;
        bool wasCreated = false;
        CThing* weapon;
        if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon.Get()->GetDefGlobalIndex() == (unsigned long)weaponDefIndex)
            weapon = PActiveMeleeWeapon.Get();
        else if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon.Get()->GetDefGlobalIndex() == (unsigned long)weaponDefIndex)
            weapon = PActiveRangedWeapon.Get();
        else {
            weapon = CreateInventoryObject(weaponDefIndex);
            wasCreated = true;
        }
        const CTCObjectAugmentations* augmentations;
        if (weapon->PeekTC(&augmentations))
            freeSlot = augmentations->GetFreeSlot();
        if (wasCreated) {
            CTCInventoryItem* item = 0;
            weapon->GetTC(&item);
            item->ReturnToInventory();
        }
        return freeSlot;
    }
};
int main()
{
    CTCObjectAugmentations a1 = { 2 }, a2 = { 0 }, a3 = { 1 };
    CThing sword = { 10, &a1, { 0 } }, bow = { 20, &a2, { 0 } }, temp = { 0, &a3, { 0 } };
    CTCInventoryWeapons w; w.PActiveMeleeWeapon.item = &sword; w.PActiveRangedWeapon.item = &bow; w.created = &temp; w.creations = 0;
    int bad = 0;
    bad |= w.GetFreeAugmentationSlotForWeapon(10) != 2 || w.creations != 0;
    bad |= w.GetFreeAugmentationSlotForWeapon(20) != 0 || w.creations != 0;
    bad |= w.GetFreeAugmentationSlotForWeapon(30) != 1 || w.creations != 1 || temp.item.returned != 1 || temp.defIndex != 30;
    temp.augmentations = 0;
    bad |= w.GetFreeAugmentationSlotForWeapon(31) != -1 || temp.item.returned != 2;
    w.PActiveMeleeWeapon.item = 0;
    bad |= w.GetFreeAugmentationSlotForWeapon(10) != -1 || w.creations != 3;
    if (bad) { std::printf("005c4671_TEST FAIL\n"); return 1; }
    std::printf("005c4671_TEST PASS\n");
    return 0;
}