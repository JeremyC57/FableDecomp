#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct CTCBase { long type; };
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
struct CThing {
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
struct CCharString { const char* m_text; CCharString(const char* text, long length) : m_text(text) { (void)length; } };
struct CTCParamsBase;
struct CTCBerserk { int m_started; CTCBerserk() : m_started(0) { } void CreateStartingEffect() { ++m_started; } };
struct CTCStrengthBoost : CTCBase { unsigned char m_active; long m_boostLevel; CTCStrengthBoost() : m_active(0), m_boostLevel(0) { } };
static CTCBerserk g_berserk; static const char* g_addedName = 0; static bool g_addedActivate = false; static CTCParamsBase* g_addedParams = (CTCParamsBase*)1;
struct CCreatureThing : CThing {
    CTCBerserk* AddTC(const CCharString& name, bool activate, CTCParamsBase* params) { g_addedName = name.m_text; g_addedActivate = activate; g_addedParams = params; return &g_berserk; }
};
static int g_baseInit = 0;
struct CCombatActionBase { virtual ~CCombatActionBase() { } virtual void Initialise() { ++g_baseInit; } CBaseIntelligentPointer PCreature; };
struct CCreatureAction_CastStrengthBerserk : CCombatActionBase { virtual void Initialise(); };
void CCreatureAction_CastStrengthBerserk::Initialise()
{
    CCombatActionBase::Initialise();
    CTCBerserk* pBerserk;
    {
        CCharString name("CTCBerserk", -1);
        pBerserk = ((CCreatureThing*)PCreature.GetPItem())->AddTC(name, true, 0);
    }
    pBerserk->CreateStartingEffect();
    CTCStrengthBoost* pBoost;
    if (PCreature.GetPItem()->GetInterface(&pBoost, 167)) {
        pBoost->m_active = 1;
        pBoost->m_boostLevel = 8;
    }
}
int main()
{
    CCreatureThing creature; CTCStrengthBoost boost; creature.Add(167, &boost); creature.Add(100, 0);
    CCreatureAction_CastStrengthBerserk action; action.PCreature.m_pItem = &creature;
    action.Initialise();
    bool ok = g_baseInit == 1 && std::strcmp(g_addedName, "CTCBerserk") == 0 && g_addedActivate && g_addedParams == 0
        && g_berserk.m_started == 1 && boost.m_active == 1 && boost.m_boostLevel == 8;
    CCreatureThing bare; action.PCreature.m_pItem = &bare; action.Initialise();
    ok = ok && g_berserk.m_started == 2 && boost.m_boostLevel == 8;
    if (!ok) { std::printf("008e6400_TEST FAIL\n"); return 1; }
    std::printf("008e6400_TEST PASS\n");
    return 0;
}
