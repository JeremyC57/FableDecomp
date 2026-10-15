// Behaviour test for CTCOwnedEntity::InitialActivate @ 0x007E8820: base first, then the owner looked up by UID when set.
#include <cstdio>
struct CThing { long id; };
static CThing g_thing = { 42 };
static int g_baseCalls = 0, g_lookups = 0;
static unsigned long g_lastLow = 0;
struct CGame {
    CThing* GetThingFromUID(unsigned long low, unsigned long high) { ++g_lookups; g_lastLow = low; return high == 0 && low == 9 ? &g_thing : 0; }
};
static CGame g_game;
struct CTCOwnedEntity {
    unsigned long OwnerUIDLow, OwnerUIDHigh;
    CThing* m_owner;
    int m_sets;
    void BaseInitialActivate() { ++g_baseCalls; }
    CGame* GetGame() { return &g_game; }
    void SetPOwner(CThing* owner) { m_owner = owner; ++m_sets; }
    void InitialActivate()
    {
        BaseInitialActivate();
        if ((OwnerUIDLow | OwnerUIDHigh) != 0) SetPOwner(GetGame()->GetThingFromUID(OwnerUIDLow, OwnerUIDHigh));
    }
};
int main()
{
    CTCOwnedEntity e; e.OwnerUIDLow = 0; e.OwnerUIDHigh = 0; e.m_owner = 0; e.m_sets = 0;
    e.InitialActivate();
    if (g_baseCalls != 1 || g_lookups != 0 || e.m_sets != 0) { std::printf("007e8820_TEST FAIL\n"); return 1; }
    e.OwnerUIDLow = 9; e.InitialActivate();
    if (g_baseCalls != 2 || g_lookups != 1 || e.m_owner != &g_thing || g_lastLow != 9) { std::printf("007e8820_TEST FAIL\n"); return 1; }
    std::printf("007e8820_TEST PASS\n");
    return 0;
}
