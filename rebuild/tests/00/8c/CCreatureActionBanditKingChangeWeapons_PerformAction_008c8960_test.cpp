#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
class CTCBase { public: long type; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CInterfaceMap::SEntry m_entries[4];
    long m_entryCount;
    CThing() : m_entryCount(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_entries; m_interfaces.last = m_entries; }
    void Add(long type, CTCBase* component)
    {
        m_presentBits[type >> 5] |= (1UL << (type & 31));
        int i = m_entryCount;
        while (i > 0 && m_entries[i - 1].type > type) { m_entries[i] = m_entries[i - 1]; --i; }
        m_entries[i].type = type; m_entries[i].component = component;
        ++m_entryCount; m_interfaces.last = m_entries + m_entryCount;
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
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
};
class CTCBoss : public CTCBase { public: long NextWeapon; long GetNextWeapon() { return NextWeapon; } };
static int g_created = 0; static CThing* g_createdFor[2]; static long g_createdDef[2];
class CThingCreatureBase { public: static void FASTCALL CreateWeaponForThingFromDef(CThing& thing, long def); };
void FASTCALL CThingCreatureBase::CreateWeaponForThingFromDef(CThing& thing, long def) { if (g_created < 2) { g_createdFor[g_created] = &thing; g_createdDef[g_created] = def; } ++g_created; }
class CCombatActionBase { public: virtual ~CCombatActionBase() { } CBaseIntelligentPointer PCreature; };
class CCreatureAction_BanditKingChangeWeapons : public CCombatActionBase { public: void PerformAction(); };
void CCreatureAction_BanditKingChangeWeapons::PerformAction()
{
    CTCBoss* pBoss;
    if (PCreature.GetPItem()->GetInterface(&pBoss, 173)) {
        long def = pBoss->GetNextWeapon();
        CThingCreatureBase::CreateWeaponForThingFromDef(*PCreature.GetPItem(), def);
        long def2 = pBoss->GetNextWeapon();
        CThingCreatureBase::CreateWeaponForThingFromDef(*PCreature.GetPItem(), def2);
    }
}
int main()
{
    CThing creature; CTCBoss weapons; weapons.NextWeapon = 4242; creature.Add(173, &weapons); creature.Add(1, 0);
    CCreatureAction_BanditKingChangeWeapons action; action.PCreature.m_pItem = &creature;
    action.PerformAction();
    bool ok = g_created == 2 && g_createdFor[0] == &creature && g_createdFor[1] == &creature && g_createdDef[0] == 4242 && g_createdDef[1] == 4242;
    CThing bare; action.PCreature.m_pItem = &bare; action.PerformAction();
    ok = ok && g_created == 2;
    if (!ok) { std::printf("008c8960_TEST FAIL\n"); return 1; }
    std::printf("008c8960_TEST PASS\n");
    return 0;
}