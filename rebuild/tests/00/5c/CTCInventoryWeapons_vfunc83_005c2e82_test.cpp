#include <cstdio>
// Behaviour of CTCInventoryWeapons::vfunc_83 (0x005C2E82).
struct CThing { unsigned short DefIndex; long m_limbo; void SetInLimbo(bool inLimbo) { if (inLimbo) ++m_limbo; } };
struct CIntelligentPointer { CThing* m_p; };
struct CTCInventoryBase_Retail { long m_baseCalls; void vfunc_83(CThing* thing) { ++m_baseCalls; } };
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    CIntelligentPointer PActiveMeleeWeapon; CIntelligentPointer PActiveRangedWeapon;
    long ConfiscatedActiveMeleeWeaponDefIndex; long ConfiscatedActiveRangedWeaponDefIndex;
    static bool IsThing(CThing* thing, CIntelligentPointer* pointer) { return thing == pointer->m_p; }
    void vfunc_83(CThing* thing)
    {
        if (IsThing(thing, &PActiveMeleeWeapon) || IsThing(thing, &PActiveRangedWeapon)
            || thing->DefIndex == ConfiscatedActiveMeleeWeaponDefIndex || thing->DefIndex == ConfiscatedActiveRangedWeaponDefIndex)
            thing->SetInLimbo(true);
        else
            CTCInventoryBase_Retail::vfunc_83(thing);
    }
};
int main()
{
    CThing melee = { 1, 0 }, ranged = { 2, 0 }, conf = { 7, 0 }, other = { 9, 0 };
    CTCInventoryWeapons w; w.m_baseCalls = 0; w.PActiveMeleeWeapon.m_p = &melee; w.PActiveRangedWeapon.m_p = &ranged;
    w.ConfiscatedActiveMeleeWeaponDefIndex = 0; w.ConfiscatedActiveRangedWeaponDefIndex = 7;
    w.vfunc_83(&melee); w.vfunc_83(&ranged); w.vfunc_83(&conf); w.vfunc_83(&other);
    int bad = (melee.m_limbo != 1 || ranged.m_limbo != 1 || conf.m_limbo != 1 || other.m_limbo != 0 || w.m_baseCalls != 1);
    if (bad) { std::printf("005c2e82_TEST FAIL\n"); return 1; }
    std::printf("005c2e82_TEST PASS\n");
    return 0;
}