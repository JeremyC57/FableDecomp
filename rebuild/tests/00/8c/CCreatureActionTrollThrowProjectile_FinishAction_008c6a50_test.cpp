// Behaviour test for 0x008C6A50 CCreatureAction_TrollThrowProjectile::FinishAction: runs the reconstructed body over stubbed callees;
// checks the carried-shot kill guard, the combat word clear and the base FinishAction.

#include <cstdio>
#include <cstring>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
struct CThing {
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char m_flags91; float m_health;
    CInterfaceMap::SEntry m_entries[4]; int m_count;
    CThing() : m_flags91(0), m_health(0), m_count(0)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0;
    }
    void Add(long type, void* component)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_entries[m_count].type = type; m_entries[m_count].component = (CTCBase*)component; ++m_count;
        m_interfaces.first = m_entries; m_interfaces.last = m_entries + m_count;
    }
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
};
static int g_kills = 0;
struct CThingK : CThing { void Kill(bool s) { if (s) ++g_kills; } };
struct CTCCarrying { CThing* carried; bool IsCarryingThing(CThing* t) { return t == carried; } };
struct CTCCombat { long m_word7c; };
struct CBaseIntelligentPointer { CThingK* m_pItem; CThingK* GetPItem() const { return m_pItem; } };
static int g_base = 0;
struct CCombatActionBase { virtual void FinishAction() { ++g_base; } CBaseIntelligentPointer PCreature; };
struct CCreatureAction_TrollThrowProjectile : CCombatActionBase { CBaseIntelligentPointer PShot; virtual void FinishAction(); };
void CCreatureAction_TrollThrowProjectile::FinishAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
        if (PShot.GetPItem() && !(PShot.GetPItem()->m_flags91 & 1) && carrying->IsCarryingThing(PShot.GetPItem()))
            PShot.GetPItem()->Kill(true);
    }
    CTCCombat* combat;
    if (PCreature.GetPItem()->GetInterface(&combat, 0x30))
        combat->m_word7c = 0;
    CCombatActionBase::FinishAction();
}
int main()
{
    CThingK shot; shot = CThingK(); CTCCarrying car; car.carried = &shot; CTCCombat cb; cb.m_word7c = 7;
    CThingK troll; troll = CThingK(); troll.Add(0x30, &cb); troll.Add(0x46, &car);
    CCreatureAction_TrollThrowProjectile a; a.PCreature.m_pItem = &troll; a.PShot.m_pItem = &shot;
    a.FinishAction();
    bool ok = g_kills == 1 && cb.m_word7c == 0 && g_base == 1;
    car.carried = 0; cb.m_word7c = 3; a.FinishAction();          // not carried any more: no kill
    ok = ok && g_kills == 1 && cb.m_word7c == 0 && g_base == 2;
    car.carried = &shot; shot.m_flags91 = 1; a.FinishAction();  // dead shot: no kill
    ok = ok && g_kills == 1;
    if (!ok) { std::printf("008c6a50_TEST FAIL\n"); return 1; }
    std::printf("008c6a50_TEST PASS\n");
    return 0;
}