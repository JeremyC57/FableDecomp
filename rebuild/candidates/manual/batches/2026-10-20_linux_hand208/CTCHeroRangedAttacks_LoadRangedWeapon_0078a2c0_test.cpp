// Behaviour test for CTCHeroRangedAttacks::LoadRangedWeapon (0x0078A2C0): the load action is started only for a
// live carried thing with a weapon component that is a projectile weapon, not loaded, while not already loading.
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    const SEntry* LowerBound(const long* type) const { const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry entries[4];
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = entries; m_flags91 = 0; }
    void Add(long type, void* comp) { m_presentBits[type >> 5] |= 1UL << (type & 31); m_interfaces.last->type = type; m_interfaces.last->component = (CTCBase*)comp; ++m_interfaces.last; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> T* FindTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type) it = map->last;
        return (T*)it->component;
    }
};
inline bool IS_THING_ALIVE(const CThing* thing) { if (thing != 0) { if (!(thing->m_flags91 & 1)) return true; } return false; }
struct CTCCarrying { CThing* InHand; CThing* GetThingInPrimarySlot() { return InHand; } };
struct CTCWeapon {
    bool Projectile, Loaded;
    bool IsProjectileWeapon() { return Projectile; }
    bool IsProjectileWeaponLoaded() { return Loaded; }
};
struct CCreatureActionBase { CThing* Weapon; };
static int g_actions = 0; static CThing* g_actionWeapon = 0; static int g_live = 0;
struct CThingCreatureBase { bool SetCurrentAction(const CCreatureActionBase& a) { ++g_actions; g_actionWeapon = a.Weapon; return true; } };
struct CCreatureAction_HeroLoadRangedWeapon : CCreatureActionBase {
    CCreatureAction_HeroLoadRangedWeapon(CThingCreatureBase*, CThing* weapon) { Weapon = weapon; ++g_live; }
    ~CCreatureAction_HeroLoadRangedWeapon() { --g_live; }
};
struct CTCHeroRangedAttacks {
    CThing* ParentThing; CThingCreatureBase* PParentCreature; bool Loading;
    void LoadRangedWeapon();
};
void CTCHeroRangedAttacks::LoadRangedWeapon()
{
    CTCCarrying* carrying;
    ParentThing->GetInterface(&carrying, 0x46);
    CThing* weapon = carrying->GetThingInPrimarySlot();
    if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)) {
        CTCWeapon* tc = weapon->FindTC<CTCWeapon>(0x48);
        if (tc->IsProjectileWeapon() && !tc->IsProjectileWeaponLoaded() && !Loading)
            PParentCreature->SetCurrentAction(CCreatureAction_HeroLoadRangedWeapon(PParentCreature, weapon));
    }
}
int main()
{
    CThing hero; hero.Init(); CThing bow; bow.Init();
    CTCCarrying carrying; carrying.InHand = 0; hero.Add(0x46, &carrying);
    CThingCreatureBase creature;
    CTCHeroRangedAttacks r; r.ParentThing = &hero; r.PParentCreature = &creature; r.Loading = false;
    r.LoadRangedWeapon();                       // nothing in hand
    carrying.InHand = &bow; r.LoadRangedWeapon();   // no weapon component
    CTCWeapon w; w.Projectile = true; w.Loaded = false; bow.Add(0x48, &w);
    bow.m_flags91 = 1; r.LoadRangedWeapon();    // dead
    bow.m_flags91 = 0; r.Loading = true; r.LoadRangedWeapon();
    r.Loading = false; w.Loaded = true; r.LoadRangedWeapon();
    w.Loaded = false; w.Projectile = false; r.LoadRangedWeapon();
    if (g_actions != 0) { std::printf("0078a2c0_TEST FAIL\n"); return 1; }
    w.Projectile = true; r.LoadRangedWeapon();
    if (g_actions != 1 || g_actionWeapon != &bow || g_live != 0) { std::printf("0078a2c0_TEST FAIL\n"); return 1; }
    std::printf("0078a2c0_TEST PASS\n");
    return 0;
}
