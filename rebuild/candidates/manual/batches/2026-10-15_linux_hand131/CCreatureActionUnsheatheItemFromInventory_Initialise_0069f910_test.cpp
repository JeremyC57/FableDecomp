// Behaviour test for 0x0069F910 CCreatureAction_UnsheatheItemFromInventory::Initialise: the base always runs; only a creature
// with the ranged-attacks component (type 0x81) gets its aiming cancelled, projectile mode reset and player mode 0x22 removed.
#include <cstdio>
static int g_base = 0, g_cancel = 0, g_reset = 0, g_removedMode = -1;
struct CPlayer { void RemoveMode(int m) { g_removedMode = m; } };
struct CRanged { void CancelAimingWeapon() { ++g_cancel; } void ResetProjectileWeaponMode() { ++g_reset; } };
struct CThing {
    unsigned int m_bits[9];
    CRanged* m_ranged;
    CPlayer m_player;
    CPlayer* GetPlayer() { return &m_player; }
    bool GetInterface(CRanged** out, int type)
    {
        if (m_bits[type >> 5] & (1u << (type & 31))) { *out = m_ranged; return true; }
        return false;
    }
};
struct CAction {
    CThing* m_creature;
    CThing* GetPItem() { return m_creature; }
    void BaseInitialise() { ++g_base; }
    void Initialise()
    {
        BaseInitialise();
        CRanged* ranged;
        if (GetPItem()->GetInterface(&ranged, 0x81)) {
            ranged->CancelAimingWeapon();
            ranged->ResetProjectileWeaponMode();
            GetPItem()->GetPlayer()->RemoveMode(0x22);
        }
    }
};
int main()
{
    CRanged r; CThing t; for (int i = 0; i < 9; ++i) t.m_bits[i] = 0; t.m_ranged = &r;
    CAction a; a.m_creature = &t;
    a.Initialise();
    if (g_base != 1 || g_cancel != 0 || g_removedMode != -1) { std::printf("0069f910_TEST FAIL\n"); return 1; }
    t.m_bits[4] = 2;    // type 0x81 = dword 4, bit 1
    a.Initialise();
    if (g_base != 2 || g_cancel != 1 || g_reset != 1 || g_removedMode != 0x22) { std::printf("0069f910_TEST FAIL\n"); return 1; }
    std::printf("0069f910_TEST PASS\n");
    return 0;
}
