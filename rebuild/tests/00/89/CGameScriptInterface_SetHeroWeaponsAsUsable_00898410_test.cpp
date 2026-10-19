// Behaviour test for 0x00898410 CGameScriptInterface::SetHeroWeaponsAsUsable: dead/absent hero, disable, enable in a
// region whose def prevents weapons, enable in a free region, null def; def reference counting.
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    virtual ~CThing() {}
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
    CInterfaceMap::SEntry m_entries[8];
    CThing() : m_dead(0), m_91_1(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = m_entries; }
    void Add(long type, void* tc)   // keep sorted
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type; it->component = (CTCBase*)tc; ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> T* GetTC(long interfaceType)
    {
        T* tc = 0;
        GetInterface(&tc, interfaceType);
        return tc;
    }
};
static int g_set = 0, g_setValue = -1, g_deleted = 0;
struct CTCHero {
    void SetWeaponsAsUsable(bool usable) { ++g_set; g_setValue = usable; WeaponsUsable = usable; }
    bool WeaponsUsable, WillUsable, WeaponUseOverriddenByScripts, WillUseOverriddenByScripts;
};
struct CPlayer { CThing* hero; CThing* GetHeroThing() { return hero; } };
struct CPlayerManager { CPlayer* player; CPlayer* GetMainPlayer() { return player; } };
struct CRegionDef {
    virtual ~CRegionDef() {}
    virtual void DeleteThis() { ++g_deleted; }
    long m_refCount;
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) DeleteThis(); }
    bool PreventHeroWeaponUse;
};
template<class T> struct CCountedPointer {
    T* Data;
    CCountedPointer(T* data) : Data(data) { if (Data) Data->AddRef(); }
    ~CCountedPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};
struct CRegion { CCountedPointer<CRegionDef> m_pDef; CRegion() : m_pDef(0) {} };
struct CWorldMap {
    CRegion regions[3]; long current;
    long GetCurrentRegionNumber() { return current; }
    CRegion& GetRegionEntry(long region) { return regions[region]; }
};
struct CWorld { CWorldMap map; virtual CWorldMap* GetWorldMap() { return &map; } virtual ~CWorld() {} };
struct CGameScriptInterface {
    CWorld* World;
    CPlayerManager* PlayerManager;
    void SetHeroWeaponsAsUsable(bool usable) const;
};
void CGameScriptInterface::SetHeroWeaponsAsUsable(bool usable) const
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero && !hero->m_dead) {
        CTCHero* heroTC = hero->GetTC<CTCHero>(0x29);
        heroTC->WeaponUseOverriddenByScripts = !usable;
        if (usable) {
            CCountedPointer<CRegionDef> def(World->GetWorldMap()->GetRegionEntry(World->GetWorldMap()->GetCurrentRegionNumber()).m_pDef.Data);
            if (!def.IsNull()) {
                if (!def->PreventHeroWeaponUse)
                    heroTC->SetWeaponsAsUsable(true);
            }
        } else {
            heroTC->SetWeaponsAsUsable(false);
        }
    }
}
int main()
{
    CThing hero; CTCHero tc = { true, false, false, false }; hero.Add(5, 0); hero.Add(0x29, &tc); hero.Add(0x40, 0);
    CPlayer player = { 0 }; CPlayerManager pm = { &player };
    CWorld world;
    CRegionDef freeDef; freeDef.m_refCount = 1; freeDef.PreventHeroWeaponUse = false;
    CRegionDef townDef; townDef.m_refCount = 1; townDef.PreventHeroWeaponUse = true;
    world.map.regions[1].m_pDef.Data = &freeDef; world.map.regions[2].m_pDef.Data = &townDef;
    CGameScriptInterface gsi = { &world, &pm };
    int fail = 0;
    gsi.SetHeroWeaponsAsUsable(false);                 // no hero
    if (g_set != 0) fail |= 1;
    player.hero = &hero; hero.m_dead = 1;
    gsi.SetHeroWeaponsAsUsable(false);                 // dead hero
    if (g_set != 0) fail |= 2;
    hero.m_dead = 0;
    gsi.SetHeroWeaponsAsUsable(false);
    if (g_set != 1 || g_setValue != 0 || !tc.WeaponUseOverriddenByScripts) fail |= 4;
    world.map.current = 2;                             // weapons prevented
    gsi.SetHeroWeaponsAsUsable(true);
    if (g_set != 1 || tc.WeaponUseOverriddenByScripts || townDef.m_refCount != 1) fail |= 8;
    world.map.current = 1;
    gsi.SetHeroWeaponsAsUsable(true);
    if (g_set != 2 || g_setValue != 1 || freeDef.m_refCount != 1 || g_deleted) fail |= 16;
    world.map.current = 0;                             // no def
    gsi.SetHeroWeaponsAsUsable(true);
    if (g_set != 2) fail |= 32;
    world.map.regions[1].m_pDef.Data = 0; world.map.regions[2].m_pDef.Data = 0;
    if (fail) { std::printf("00898410_TEST FAIL\n"); return 1; }
    std::printf("00898410_TEST PASS\n");
    return 0;
}