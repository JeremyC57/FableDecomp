// Behaviour test for 0x0072cd00 CTCSpecialAbilities::SetupDivineWrath (copy of the source logic over spy actions).
#include <cstdio>
#include <cstring>
class CTCBase { public: virtual ~CTCBase() {} };
class CCharString { public: const char* text; };
static CCharString g_CCharString_013bad30 = { "SHEATHE_SLOT" };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
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
static CThing* g_primary = 0;
class CTCCarrying : public CTCBase { public: CThing* GetThingInPrimarySlot() { return g_primary; } };
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CIntelligentPointerThing() : m_pItem(0) {}
    CIntelligentPointerThing(CThing* item) : m_pItem(item) {}
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o) { m_pItem = o.m_pItem; return *this; }
};
class CThingCreatureBase;
class CCreatureActionBase { public: int kind; virtual ~CCreatureActionBase() {} };
static int g_combatDtors = 0, g_sheatheDtors = 0;
class CCombatActionBase : public CCreatureActionBase { public:
    CIntelligentPointerThing PItem;
    ~CCombatActionBase() { ++g_combatDtors; }
};
class CCreatureAction_CastDivineWrathSpell : public CCombatActionBase { public:
    CThingCreatureBase* owner;
    CCreatureAction_CastDivineWrathSpell(CThingCreatureBase* o) : owner(o) { kind = 1; }
};
static const char* g_slot = 0; static CCreatureActionBase* g_next = 0;
class CCreatureAction_SheatheItemToInventory_ForDivineWrath : public CCreatureActionBase { public:
    CCreatureAction_SheatheItemToInventory_ForDivineWrath(CThingCreatureBase*, const CCharString& slot, CCreatureActionBase* next, long)
    { kind = 2; g_slot = slot.text; g_next = next; }
    ~CCreatureAction_SheatheItemToInventory_ForDivineWrath() { ++g_sheatheDtors; }
};
static int g_lastKind = 0;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action) { g_lastKind = action.kind; return action.kind == 2; }
};
class CTCSpecialAbilities { public:
    CThingCreatureBase* Creature;
    bool SetupDivineWrath();
};
bool CTCSpecialAbilities::SetupDivineWrath()
{
    CTCCarrying* carrying;
    Creature->GetInterface(&carrying, 0x46);
    CThing* item = carrying->GetThingInPrimarySlot();
    if (item && !(item->m_flags91 & 1)) {
        CCreatureAction_CastDivineWrathSpell* cast = new CCreatureAction_CastDivineWrathSpell(Creature);
        cast->PItem = item;
        CCreatureAction_SheatheItemToInventory_ForDivineWrath sheathe(Creature, g_CCharString_013bad30, cast, 0);
        return Creature->SetCurrentAction(sheathe);
    }
    return Creature->SetCurrentAction(CCreatureAction_CastDivineWrathSpell(Creature));
}
static int fail() { std::printf("0072cd00_TEST FAIL\n"); return 1; }
int main()
{
    CTCCarrying carrying;
    CInterfaceMap::SEntry e[1] = { { 0x46, &carrying } };
    CThingCreatureBase hero; std::memset(hero.m_presentBits, 0, sizeof hero.m_presentBits);
    hero.m_presentBits[0x46 >> 5] = 1UL << (0x46 & 31); hero.m_interfaces.first = e; hero.m_interfaces.last = e + 1;
    CTCSpecialAbilities sa; sa.Creature = &hero;
    if (sa.SetupDivineWrath() || g_lastKind != 1 || g_combatDtors != 1) return fail();          // nothing carried: cast directly
    CThing sword; std::memset(&sword, 0, sizeof sword);
    g_primary = &sword;
    if (!sa.SetupDivineWrath() || g_lastKind != 2 || g_sheatheDtors != 1 || !g_next || std::strcmp(g_slot, "SHEATHE_SLOT") != 0) return fail();
    if (((CCombatActionBase*)g_next)->PItem.m_pItem != &sword) return fail();       // the queued cast remembers the item
    delete (CCreatureAction_CastDivineWrathSpell*)g_next;
    sword.m_flags91 = 1;                                                              // dead item: cast directly
    if (sa.SetupDivineWrath() || g_lastKind != 1) return fail();
    std::printf("0072cd00_TEST PASS\n");
    return 0;
}