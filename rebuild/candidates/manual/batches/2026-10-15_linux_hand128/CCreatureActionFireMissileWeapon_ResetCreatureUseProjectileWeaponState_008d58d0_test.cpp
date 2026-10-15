// Behaviour test for 0x008D58D0 CCreatureAction_FireMissileWeapon::ResetCreatureUseProjectileWeaponState: for a live
// creature ReleaseMissile runs with a null-initialised missile slot and the ranged-attacks component (0x81) cancels
// aiming when present; a dead or missing creature does nothing.
#include <cstdio>

struct CTCBase { int calls; void* arg; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    int m_killed;
    void Kill(bool removeNow) { m_killed = removeNow ? 2 : 1; m_flags91 |= 1; }
    template <class T> void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
    void Init()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0; m_flags91 = 0; m_killed = 0;
    }
    void AddTC(CInterfaceMap::SEntry* entries, int n, long type)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_interfaces.first = entries; m_interfaces.last = entries + n;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCHeroRangedAttacks : CTCBase { void CancelAimingWeapon() { ++calls; } };
struct CCreatureAction_FireMissileWeapon {
    CBaseIntelligentPointer PCreature; int m_releases; CThing* m_seenMissile;
    void ReleaseMissile(CThing** missile, CThing** target, long* globalIndex)
    { ++m_releases; m_seenMissile = *missile; *target = 0; *globalIndex = -1; }
    void ResetCreatureUseProjectileWeaponState()
    {
        if (IS_THING_ALIVE(PCreature)) {
            {
                CThing* missile = 0; CThing* target; long globalIndex;
                ReleaseMissile(&missile, &target, &globalIndex);
            }
            CThing* c = PCreature.GetPItem();
            if (c->m_presentBits[0x81 >> 5] & (1UL << (0x81 & 31))) {
                CTCHeroRangedAttacks* ranged; c->GetInterface(&ranged, 0x81);
                ranged->CancelAimingWeapon();
            }
        }
    }
};
int main()
{
    CTCHeroRangedAttacks ranged; ranged.calls = 0;
    CInterfaceMap::SEntry e[1] = { { 0x81, &ranged } };
    CThing creature; creature.Init();
    CCreatureAction_FireMissileWeapon a; a.PCreature.m_pItem = &creature; a.m_releases = 0; a.m_seenMissile = &creature;
    a.ResetCreatureUseProjectileWeaponState();          // no ranged component: release only
    bool ok = a.m_releases == 1 && a.m_seenMissile == 0 && ranged.calls == 0;
    creature.AddTC(e, 1, 0x81); a.ResetCreatureUseProjectileWeaponState();
    ok = ok && a.m_releases == 2 && ranged.calls == 1;
    creature.m_flags91 = 1; a.ResetCreatureUseProjectileWeaponState();
    ok = ok && a.m_releases == 2 && ranged.calls == 1;
    a.PCreature.m_pItem = 0; a.ResetCreatureUseProjectileWeaponState();
    ok = ok && a.m_releases == 2;
    if (!ok) { std::printf("008d58d0_TEST FAIL\n"); return 1; }
    std::printf("008d58d0_TEST PASS\n");
    return 0;
}
