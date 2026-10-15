#include <cstdio>
// Behaviour of CTCInventoryWeapons::ReturnAllConfiscatedItems (0x005CB742).
struct CComponent;
static long g_equipped[4]; static long g_n = 0;
struct CTCInventoryBase_Retail { long m_baseCalls; void ReturnAllConfiscatedItems() { ++m_baseCalls; } };
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    long ConfiscatedActiveMeleeWeaponDefIndex; long ConfiscatedActiveRangedWeaponDefIndex;
    void SetThingAsActiveWeapon(long defIndex, CComponent* menuEntry) { if (menuEntry == 0) g_equipped[g_n++] = defIndex; }
    void ReturnAllConfiscatedItems()
    {
        CTCInventoryBase_Retail::ReturnAllConfiscatedItems();
        if (ConfiscatedActiveMeleeWeaponDefIndex > 0) {
            SetThingAsActiveWeapon(ConfiscatedActiveMeleeWeaponDefIndex, 0);
            ConfiscatedActiveMeleeWeaponDefIndex = 0;
        }
        if (ConfiscatedActiveRangedWeaponDefIndex > 0) {
            SetThingAsActiveWeapon(ConfiscatedActiveRangedWeaponDefIndex, 0);
            ConfiscatedActiveRangedWeaponDefIndex = 0;
        }
    }
};
int main()
{
    CTCInventoryWeapons w; w.m_baseCalls = 0; w.ConfiscatedActiveMeleeWeaponDefIndex = 12; w.ConfiscatedActiveRangedWeaponDefIndex = -1;
    w.ReturnAllConfiscatedItems();
    int bad = (w.m_baseCalls != 1 || g_n != 1 || g_equipped[0] != 12 || w.ConfiscatedActiveMeleeWeaponDefIndex != 0 || w.ConfiscatedActiveRangedWeaponDefIndex != -1);
    w.ConfiscatedActiveRangedWeaponDefIndex = 5; w.ReturnAllConfiscatedItems();
    bad |= (g_n != 2 || g_equipped[1] != 5 || w.ConfiscatedActiveRangedWeaponDefIndex != 0);
    if (bad) { std::printf("005cb742_TEST FAIL\n"); return 1; }
    std::printf("005cb742_TEST PASS\n");
    return 0;
}
