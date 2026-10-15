#include <cstdio>
struct CTCInventoryBase {
    int confiscated;
    virtual ~CTCInventoryBase() {}
    virtual void RemoveAllConfiscatedItems() { confiscated = 0; }
};
struct CTCInventoryWeapons : CTCInventoryBase {
    long ConfiscatedActiveMeleeWeaponDefIndex, ConfiscatedActiveRangedWeaponDefIndex;
    virtual void RemoveAllConfiscatedItems()
    {
        CTCInventoryBase::RemoveAllConfiscatedItems();
        ConfiscatedActiveMeleeWeaponDefIndex = 0;
        ConfiscatedActiveRangedWeaponDefIndex = 0;
    }
};
int main()
{
    CTCInventoryWeapons w; w.confiscated = 5; w.ConfiscatedActiveMeleeWeaponDefIndex = 12; w.ConfiscatedActiveRangedWeaponDefIndex = 34;
    CTCInventoryBase* b = &w; b->RemoveAllConfiscatedItems();
    if (w.confiscated != 0 || w.ConfiscatedActiveMeleeWeaponDefIndex != 0 || w.ConfiscatedActiveRangedWeaponDefIndex != 0) { std::printf("005c2e68_TEST FAIL\n"); return 1; }
    std::printf("005c2e68_TEST PASS\n");
    return 0;
}