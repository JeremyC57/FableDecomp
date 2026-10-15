// Behaviour test for 0x0069F480 CCreatureAction_SheatheItemToInventory::Initialise.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
static void Log(int v) { if (g_n < 8) g_log[g_n++] = v; }
struct CPlayer { int m_removed; void RemoveMode(int mode) { m_removed = mode; Log(4); } };
struct CTCHeroRangedAttacks { void CancelAimingWeapon() { Log(2); } void ResetProjectileWeaponMode() { Log(3); } };
struct CThing {
    unsigned int m_presentBits[9];
    CTCHeroRangedAttacks* m_ranged;
    CPlayer* m_player;
    CPlayer* GetPlayer() { return m_player; }
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_ranged; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature;
    void Initialise()
    {
        Log(1);   // CCreatureActionBase::Initialise
        CTCHeroRangedAttacks* pRanged;
        if (PCreature.GetPItem()->GetInterface(&pRanged, 0x81)) {
            pRanged->CancelAimingWeapon();
            pRanged->ResetProjectileWeaponMode();
            PCreature.GetPItem()->GetPlayer()->RemoveMode(0x22);
        }
    }
};
static int Fail() { std::printf("0069f480_TEST FAIL\n"); return 1; }
int main()
{
    CTCHeroRangedAttacks ranged; CPlayer player; player.m_removed = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_ranged = &ranged; me.m_player = &player;
    CAction a; a.PCreature.m_pItem = &me;
    a.Initialise();
    if (g_n != 1 || player.m_removed != 0) return Fail();
    me.m_presentBits[0x81 >> 5] |= 1u << (0x81 & 31);
    g_n = 0; a.Initialise();
    if (g_n != 4 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || g_log[3] != 4 || player.m_removed != 0x22) return Fail();
    std::printf("0069f480_TEST PASS\n");
    return 0;
}