// Behaviour test for 0x008CAFA0 CCreatureAction_SummonerFlameSlice::PerformAction: runs the reconstructed body over stubbed
// callees; checks the obstruction kill, the def-name lookup, the explosion spawned at the summoner and the exclusion.
#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
struct CThing;
static CThing* g_excluded = 0; static int g_excl = 0;
struct CExplosionDriver { void AddExcludedThing(CThing* t) { g_excluded = t; ++g_excl; } };
static int g_kills = 0;
struct CThing {
    unsigned char m_driverFlags; CExplosionDriver* m_pDriver; unsigned char m_flags91; C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
    void Kill(bool silent) { if (silent) ++g_kills; }
    bool GetTCDriver(CExplosionDriver** out) { if (m_driverFlags & 2) { *out = m_pDriver; return true; } return false; }
};
static CThing g_explosion; static long g_index = 0; static const C3DVector* g_at = 0; static bool g_spawn = true;
CThing* CreateThing(long defIndex, const C3DVector& position) { g_index = defIndex; g_at = &position; return g_spawn ? &g_explosion : 0; }
static char g_def[64];
struct CCharString { const char* m_pData; CCharString(const char* t, long) : m_pData(t) { } ~CCharString() { } };
struct CGameDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetDefGlobalIndexFromName(const CCharString& name) { std::strcpy(g_def, name.m_pData); return 1234; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCombatActionBase { CBaseIntelligentPointer PCreature; };
struct CCreatureAction_SummonerFlameSlice : CCombatActionBase {
    CBaseIntelligentPointer PObstruction;
    void PerformAction();
};
void CCreatureAction_SummonerFlameSlice::PerformAction()
{
    if (PObstruction.GetPItem() && !(PObstruction.GetPItem()->m_flags91 & 1))
        PObstruction.GetPItem()->Kill(true);
    long explosionIndex;
    {
        CCharString name("THING_EXPLOSION_SUMMONER_FLAME_SLICE_SPHERICAL", -1);
        explosionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* explosion = CreateThing(explosionIndex, PCreature.GetPItem()->GetPosition());
    if (explosion && !(explosion->m_flags91 & 1)) {
        CExplosionDriver* driver;
        explosion->GetTCDriver(&driver);
        driver->AddExcludedThing(PCreature.GetPItem());
    }
}
int main()
{
    CExplosionDriver drv; g_explosion = CThing(); g_explosion.m_driverFlags = 2; g_explosion.m_pDriver = &drv;
    CThing owner = CThing(), obstruction = CThing();
    CCreatureAction_SummonerFlameSlice a; a.PCreature.m_pItem = &owner; a.PObstruction.m_pItem = &obstruction;
    a.PerformAction();
    bool ok = g_kills == 1 && std::strcmp(g_def, "THING_EXPLOSION_SUMMONER_FLAME_SLICE_SPHERICAL") == 0 && g_index == 1234 &&
              g_at == &owner.m_pos && g_excl == 1 && g_excluded == &owner;
    obstruction.m_flags91 = 1; g_explosion.m_flags91 = 1;       // dead obstruction, dead explosion: no kill, no exclusion
    a.PerformAction();
    ok = ok && g_kills == 1 && g_excl == 1;
    g_spawn = false; a.PObstruction.m_pItem = 0;                // no explosion at all
    a.PerformAction();
    ok = ok && g_excl == 1;
    if (!ok) { std::printf("008cafa0_TEST FAIL\n"); return 1; }
    std::printf("008cafa0_TEST PASS\n");
    return 0;
}
