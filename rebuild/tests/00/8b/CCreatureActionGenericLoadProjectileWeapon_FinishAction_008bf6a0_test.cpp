// Behaviour test for 0x008BF6A0 CCreatureAction_GenericLoadProjectileWeapon::FinishAction: a live carried weapon with
// a weapon component (0x48) gets SetProjectileWeaponLoaded(true, 0); a dead weapon, an empty slot or a missing
// component skip it; the base FinishAction always runs.
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
struct CTCCarrying : CTCBase { CThing* slot; CThing* GetThingInPrimarySlot() { ++calls; return slot; } };
struct CTCWeapon : CTCBase { bool loaded; void SetProjectileWeaponLoaded(bool l, CThing* p) { ++calls; loaded = l; arg = p; } };
struct CCreatureAction_GenericLoadProjectileWeapon {
    CBaseIntelligentPointer PCreature; int m_baseFinishes;
    void FinishAction()
    {
        CThing* creature = PCreature.GetPItem();
        if (creature->m_presentBits[0x46 >> 5] & (1UL << (0x46 & 31))) {
            CTCCarrying* carrying; creature->GetInterface(&carrying, 0x46);
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon != 0 && !(weapon->m_flags91 & 1)) {
                if (weapon->m_presentBits[0x48 >> 5] & (1UL << (0x48 & 31))) {
                    CTCWeapon* w = 0; weapon->GetInterface(&w, 0x48);
                    w->SetProjectileWeaponLoaded(true, 0);
                }
            }
        }
        ++m_baseFinishes;
    }
};
int main()
{
    CTCCarrying carrying; carrying.calls = 0; carrying.slot = 0;
    CTCWeapon wtc; wtc.calls = 0; wtc.loaded = false; wtc.arg = (void*)1;
    CInterfaceMap::SEntry ce[1] = { { 0x46, &carrying } };
    CInterfaceMap::SEntry we[1] = { { 0x48, &wtc } };
    CThing creature; creature.Init();
    CThing weapon; weapon.Init();
    CCreatureAction_GenericLoadProjectileWeapon a; a.PCreature.m_pItem = &creature; a.m_baseFinishes = 0;
    a.FinishAction();                                   // no carrying component
    bool ok = carrying.calls == 0 && a.m_baseFinishes == 1;
    creature.AddTC(ce, 1, 0x46);
    a.FinishAction();                                   // empty slot
    ok = ok && carrying.calls == 1 && wtc.calls == 0;
    carrying.slot = &weapon; a.FinishAction();          // weapon without a weapon component
    ok = ok && wtc.calls == 0;
    weapon.AddTC(we, 1, 0x48); weapon.m_flags91 = 1; a.FinishAction();   // dead weapon
    ok = ok && wtc.calls == 0;
    weapon.m_flags91 = 0; a.FinishAction();             // loads
    ok = ok && wtc.calls == 1 && wtc.loaded && wtc.arg == 0 && a.m_baseFinishes == 5;
    if (!ok) { std::printf("008bf6a0_TEST FAIL\n"); return 1; }
    std::printf("008bf6a0_TEST PASS\n");
    return 0;
}