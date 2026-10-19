// Behaviour test for 0x008CB500 CCreatureAction_SummonerLightningOrbOutOfGroundSmash::PerformAction: runs the reconstructed
// body over stubbed callees; checks the mode-manager RemoveMode(0x5a) guard, the explosion spawn and the exclusion.
#include <cstdio>
#include <cstring>
struct C3DVector { float x, y, z; };
struct CThing;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
static unsigned long g_removed = 0; static int g_removes = 0;
struct CTCCreatureModeManager { void RemoveMode(unsigned long m) { g_removed = m; ++g_removes; } };
static CThing* g_excluded = 0; static int g_excl = 0;
struct CExplosionDriver { void AddExcludedThing(CThing* t) { g_excluded = t; ++g_excl; } };
struct CThing {
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; CExplosionDriver* m_pDriver; C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
    bool GetTCDriver(CExplosionDriver** out) { if (m_presentBits[2] & 2) { *out = m_pDriver; return true; } return false; }
};
static CThing g_explosion; static long g_index = 0; static const C3DVector* g_at = 0;
CThing* CreateThing(long defIndex, const C3DVector& position) { g_index = defIndex; g_at = &position; return &g_explosion; }
static char g_def[64];
struct CCharString { const char* m_pData; CCharString(const char* t, long) : m_pData(t) { } ~CCharString() { } };
struct CGameDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    long GetDefGlobalIndexFromName(const CCharString& name) { std::strcpy(g_def, name.m_pData); return 99; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCombatActionBase { CBaseIntelligentPointer PCreature; };
struct CCreatureAction_SummonerLightningOrbOutOfGroundSmash : CCombatActionBase { void PerformAction(); };
void CCreatureAction_SummonerLightningOrbOutOfGroundSmash::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x5a);
    long explosionIndex;
    {
        CCharString name("THING_EXPLOSION_SUMMONER_LIGHTNING_GROUND_SMASH", -1);
        explosionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* explosion = CreateThing(explosionIndex, PCreature.GetPItem()->GetPosition());
    CExplosionDriver* driver;
    explosion->GetTCDriver(&driver);
    driver->AddExcludedThing(PCreature.GetPItem());
}
int main()
{
    CTCCreatureModeManager mm; CInterfaceMap::SEntry e[1] = { { 0x31, (CTCBase*)&mm } };
    CExplosionDriver drv; g_explosion = CThing(); g_explosion.m_presentBits[2] = 2; g_explosion.m_pDriver = &drv;
    CThing owner = CThing(); owner.m_interfaces.first = e; owner.m_interfaces.last = e + 1;
    CCreatureAction_SummonerLightningOrbOutOfGroundSmash a; a.PCreature.m_pItem = &owner;
    a.PerformAction();                                           // no mode manager: no RemoveMode
    bool ok = g_removes == 0 && g_excl == 1 && g_excluded == &owner && g_index == 99 && g_at == &owner.m_pos &&
              std::strcmp(g_def, "THING_EXPLOSION_SUMMONER_LIGHTNING_GROUND_SMASH") == 0;
    owner.m_presentBits[1] = 1UL << (0x31 & 31);
    a.PerformAction();
    ok = ok && g_removes == 1 && g_removed == 0x5a && g_excl == 2;
    if (!ok) { std::printf("008cb500_TEST FAIL\n"); return 1; }
    std::printf("008cb500_TEST PASS\n");
    return 0;
}
