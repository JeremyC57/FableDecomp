// Behaviour test for CTCGhostSword::CTCGhostSword(CThing&) (retail 0x007C5560): model copy + stub callees.
#include <cstdio>
#include <cstring>

class CThing;
class CBaseObject;
class CTCBase;

static char g_lastName[64];
static long g_lastClassIndex = -1;
static long g_lastAbility = -1;

struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) {}
    ~CCharString() {}
};

class CSpecialAbilitiesGhostSwordDef {
public:
    float FadeInSecs;
};
static CSpecialAbilitiesGhostSwordDef g_def = { 1.5f };

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const
    { std::strcpy(g_lastName, className.text); g_lastClassIndex = classIndex; return 42; }
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const
    { if (index != 42) return false; out->Object = &g_def; return true; }
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

template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) {}
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};

class CTCInventoryAbilities {
public:
    long level;
    long GetAbilityLevel(long ability) { g_lastAbility = ability; return level; }
};

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

long g_framesPerSecond = 30;

class CTCBase {
public:
    CTCBase(CThing* parent) : ParentThing(parent) {}
    CThing* ParentThing;
};

class CThingAICreature;
class CTCGhostSword : public CTCBase {
public:
    CDefPointer<const CSpecialAbilitiesGhostSwordDef> PDef;
    CRetailVector< CIntelligentPointer<CThingAICreature> > Minions;
    CRetailVector< CIntelligentPointer<CThing> > MinionEffects;
    long MinionAliveTimer;
    long FadeInTimer;
    CRetailVector<long> CurFadeInTimer;
    long SpellLevel;
    CIntelligentPointer<CThing> PItemRemoved;
    bool CutSceneCancelled;
    CTCGhostSword(CThing& parent);
};

CTCGhostSword::CTCGhostSword(CThing& parent)
    : CTCBase(&parent), FadeInTimer(0), SpellLevel(0), PItemRemoved(0), CutSceneCancelled(false)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_GHOST_SWORD_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    FadeInTimer = (long)(g_framesPerSecond * PDef->FadeInSecs);
    CTCInventoryAbilities* abilities;
    if (ParentThing->GetInterface(&abilities, 0x6f))
        SpellLevel = abilities->GetAbilityLevel(0xf);
}

int main()
{
    int fail = 0;
    CTCInventoryAbilities inv = { 3 };
    CInterfaceMap::SEntry entries[1] = { { 0x6f, (CTCBase*)&inv } };
    CThing hero;
    for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    hero.m_presentBits[3] = 1UL << 15; hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1;
    CTCGhostSword sword(hero);
    if (sword.ParentThing != &hero || sword.PDef.Object != &g_def) fail = 1;
    if (std::strcmp(g_lastName, "SPECIAL_ABILITIES_GHOST_SWORD_DEF") != 0 || g_lastClassIndex != 1) fail = 2;
    if (sword.FadeInTimer != 45 || sword.SpellLevel != 3 || g_lastAbility != 0xf) fail = 3;
    if (sword.Minions._M_start || sword.MinionEffects._M_finish || sword.CurFadeInTimer._M_end || sword.PItemRemoved.m_pItem || sword.CutSceneCancelled) fail = 4;
    // no ability inventory: spell level stays 0
    CThing plain;
    for (int i = 0; i < 9; ++i) plain.m_presentBits[i] = 0;
    plain.m_interfaces.first = plain.m_interfaces.last = 0;
    CTCGhostSword other(plain);
    if (other.SpellLevel != 0 || other.FadeInTimer != 45) fail = 5;
    if (fail) { std::printf("007c5560_TEST FAIL\n"); return 1; }
    std::printf("007c5560_TEST PASS\n");
    return 0;
}
