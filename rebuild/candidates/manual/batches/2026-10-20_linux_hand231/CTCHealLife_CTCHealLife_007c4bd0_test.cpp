// Behaviour test for CTCHealLife::CTCHealLife(CThing&) (retail 0x007C4BD0): model copy + stub callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>

class CThing;
class CBaseObject;
class CTCBase;

static char g_lastName[64];
static long g_lastAbility = -1;
static int g_startCalls = 0;

struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) {}
    ~CCharString() {}
};
class CSpecialAbilitiesHealLifeDef { public: long RefCount; };
static CSpecialAbilitiesHealLifeDef g_def = { 1 };
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};
class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long) const { std::strcpy(g_lastName, className.text); return 7; }
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const { if (index == 7) out->Object = &g_def; return true; }
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    void SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end; CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) {} };
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T* _M_data; };
template <class T> struct CRetailList {
    SListNode<T>* _M_node;
    CRetailList() : _M_node(0)
    {
        SListNode<T>* node = (SListNode<T>*)std::malloc(sizeof(SListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CTCInventoryAbilities { public: long level; long GetAbilityLevel(long ability) { g_lastAbility = ability; return level; } };
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCBase { public: CTCBase(CThing* parent) : ParentThing(parent) {} CThing* ParentThing; };
class CThingCreatureBase;
class CTCHealLife : public CTCBase {
public:
    CDefPointer<const CSpecialAbilitiesHealLifeDef> PDef;
    float TotalHealthAccum;
    unsigned long LoopingSound;
    long SpellLevel;
    CIntelligentPointer<CThing> PItemRemoved;
    CRetailList< CIntelligentPointer<CThingCreatureBase> > ThingsToHeal;
    CRetailVector< CIntelligentPointer<CThing> > HealingEffects;
    CRetailVector<float> HealthIncreaseAccum;
    CIntelligentPointer<CThing> PHealingTarget;
    long spellLevelAtStart;
    CTCHealLife(CThing& parent);
    void StartHealingEffect() { ++g_startCalls; spellLevelAtStart = SpellLevel; }
};
CTCHealLife::CTCHealLife(CThing& parent)
    : CTCBase(&parent), TotalHealthAccum(0), LoopingSound(0), SpellLevel(0), PItemRemoved(0), PHealingTarget(0)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_HEAL_LIFE_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        SpellLevel = abilities->GetAbilityLevel(0xe);
    StartHealingEffect();
}
int main()
{
    int fail = 0;
    CTCInventoryAbilities inv = { 4 };
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x6f, (CTCBase*)&inv } };
    CThing hero;
    for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    hero.m_presentBits[3] = 1UL << 15; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    CTCHealLife heal(hero);
    if (heal.ParentThing != &hero || heal.PDef.Object != &g_def || std::strcmp(g_lastName, "SPECIAL_ABILITIES_HEAL_LIFE_DEF")) fail = 1;
    if (heal.SpellLevel != 4 || g_lastAbility != 0xe || g_startCalls != 1 || heal.spellLevelAtStart != 4) fail = 2;
    if (heal.TotalHealthAccum != 0.0f || heal.LoopingSound != 0 || heal.PItemRemoved.m_pItem || heal.PHealingTarget.m_pItem) fail = 3;
    if (!heal.ThingsToHeal._M_node || heal.ThingsToHeal._M_node->_M_next != heal.ThingsToHeal._M_node
        || heal.ThingsToHeal._M_node->_M_prev != heal.ThingsToHeal._M_node) fail = 4;
    if (heal.HealingEffects._M_start || heal.HealthIncreaseAccum._M_end) fail = 5;
    std::free(heal.ThingsToHeal._M_node);
    if (fail) { std::printf("007c4bd0_TEST FAIL\n"); return 1; }
    std::printf("007c4bd0_TEST PASS\n");
    return 0;
}
