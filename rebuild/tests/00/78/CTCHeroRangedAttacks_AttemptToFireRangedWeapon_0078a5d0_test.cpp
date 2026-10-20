// Behaviour test for CTCHeroRangedAttacks::AttemptToFireRangedWeapon (0x0078A5D0): when able, a live projectile
// weapon in hand queues the shot and fires it immediately if loaded.
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
static int g_fires = 0;
struct CTCHeroRangedAttacks {
    CThing* ParentThing; bool FireQueued; bool Able;
    bool IsAbleToUseRangedWeapon() { return Able; }
    void FireRangedWeapon() { ++g_fires; }
    void AttemptToFireRangedWeapon();
};
void CTCHeroRangedAttacks::AttemptToFireRangedWeapon()
{
    if (IsAbleToUseRangedWeapon()) {
        CTCCarrying* carrying;
        ParentThing->GetInterface(&carrying, 0x46);
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (IS_THING_ALIVE(weapon) && weapon->HasTC(0x48)) {
            CTCWeapon* tc = weapon->FindTC<CTCWeapon>(0x48);
            if (tc->IsProjectileWeapon()) {
                FireQueued = true;
                if (tc->IsProjectileWeaponLoaded())
                    FireRangedWeapon();
            }
        }
    }
}
int main()
{
    CThing hero; hero.Init(); CThing bow; bow.Init();
    CTCCarrying carrying; carrying.InHand = &bow; hero.Add(0x46, &carrying);
    CTCWeapon w; w.Projectile = true; w.Loaded = false; bow.Add(0x48, &w);
    CTCHeroRangedAttacks r; r.ParentThing = &hero; r.FireQueued = false; r.Able = false;
    r.AttemptToFireRangedWeapon();
    if (r.FireQueued || g_fires) { std::printf("0078a5d0_TEST FAIL\n"); return 1; }
    r.Able = true; r.AttemptToFireRangedWeapon();
    if (!r.FireQueued || g_fires) { std::printf("0078a5d0_TEST FAIL\n"); return 1; }
    w.Loaded = true; r.AttemptToFireRangedWeapon();
    if (g_fires != 1) { std::printf("0078a5d0_TEST FAIL\n"); return 1; }
    r.FireQueued = false; w.Projectile = false; r.AttemptToFireRangedWeapon();
    if (r.FireQueued || g_fires != 1) { std::printf("0078a5d0_TEST FAIL\n"); return 1; }
    w.Projectile = true; bow.m_flags91 = 1; r.AttemptToFireRangedWeapon();
    if (r.FireQueued || g_fires != 1) { std::printf("0078a5d0_TEST FAIL\n"); return 1; }
    std::printf("0078a5d0_TEST PASS\n");
    return 0;
}