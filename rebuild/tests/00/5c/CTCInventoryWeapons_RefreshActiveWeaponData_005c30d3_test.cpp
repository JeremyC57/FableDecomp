#include <cstdio>
// Behaviour of CTCInventoryWeapons::RefreshActiveWeaponData (0x005C30D3).
struct CThing { bool m_alive; long m_refreshes; };
struct CIntelligentPointer { CThing* m_p; CThing* GetPItem() { return m_p; } };
bool IS_THING_ALIVE(CIntelligentPointer* pointer) { return pointer->m_p && pointer->m_p->m_alive; }
struct CTCInventoryBase_Retail { void RefreshItemData(CThing* item) { ++item->m_refreshes; } };
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    CIntelligentPointer PActiveMeleeWeapon; CIntelligentPointer PActiveRangedWeapon;
    void RefreshActiveWeaponData()
    {
        if (IS_THING_ALIVE(&PActiveMeleeWeapon))
            RefreshItemData(PActiveMeleeWeapon.GetPItem());
        if (IS_THING_ALIVE(&PActiveRangedWeapon))
            RefreshItemData(PActiveRangedWeapon.GetPItem());
    }
};
int main()
{
    CThing melee = { true, 0 }, ranged = { false, 0 };
    CTCInventoryWeapons w; w.PActiveMeleeWeapon.m_p = &melee; w.PActiveRangedWeapon.m_p = &ranged;
    w.RefreshActiveWeaponData();
    int bad = (melee.m_refreshes != 1 || ranged.m_refreshes != 0);
    ranged.m_alive = true; w.PActiveMeleeWeapon.m_p = 0; w.RefreshActiveWeaponData();
    bad |= (melee.m_refreshes != 1 || ranged.m_refreshes != 1);
    if (bad) { std::printf("005c30d3_TEST FAIL\n"); return 1; }
    std::printf("005c30d3_TEST PASS\n");
    return 0;
}