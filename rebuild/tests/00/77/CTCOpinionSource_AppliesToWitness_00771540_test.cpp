// Behaviour test for CTCOpinionSource::AppliesToWitness (0x00771540): null witness -> false, no restricting faction -> true,
// no enemy component -> false, unknown faction name -> false, else allied ? !reversed : reversed; the name string is released.
#include <cstdio>
#include <cstring>
static int g_strings = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { ++g_strings; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CDefString { public: long Index; };
struct CDefStringTable {
    CCharString GetString(long id) { return CCharString(id == 5 ? "GOOD" : "NOBODY"); }
};
CDefStringTable g_DefStringTable;
class CFaction { public: int id; };
static CFaction g_good = { 1 };
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};
class CFactionManager { public:
    CFaction* GetPFaction(const CCharString& name) const { return std::strcmp(name.PStringData, "GOOD") == 0 ? &g_good : 0; }
};
class CWorld { public: scoped_ptr<CFactionManager> PFactionManager; };
class CMainGameComponent { public: scoped_ptr<CWorld> PWorld; };
static CFactionManager g_fm; static CWorld g_world = { { &g_fm } }; static CMainGameComponent g_mgc = { { &g_world } };
CMainGameComponent* g_pMainGameComponent = &g_mgc;
class CDefClassBase { public:
    long m_name;
    virtual CDefString GetInstantiationName() const { CDefString d = { m_name }; return d; }
};
class CFactionDef : public CDefClassBase { public: };
class CTCBase;
class CTCEnemy { public:
    const CFaction* m_ally;
    bool IsAlliedWithFaction(const CFaction* faction) const { return faction == m_ally; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingAICreature : public CThing { public: };
template <class T> class CDefPointer { public: T* Object; };
class CTCOpinionSource { public:
    CDefPointer<const CFactionDef> RestrictToFactionAndAllies;
    bool          RestrictToFactionIsReversed;
    bool AppliesToWitness(const CThingAICreature* witness) const;
};

bool CTCOpinionSource::AppliesToWitness(const CThingAICreature* witness) const
{
    if (witness) {
        const CFactionDef* faction = RestrictToFactionAndAllies.Object;
        if (!faction)
            return true;
        if (witness->HasTC(0x49)) {
            const CTCEnemy* enemy = (const CTCEnemy*)witness->GetTC(0x49);
            const CFaction* factionObject;
            {
                CCharString name = g_DefStringTable.GetString(faction->GetInstantiationName().Index);
                factionObject = g_pMainGameComponent->PWorld->PFactionManager->GetPFaction(name);
            }
            if (factionObject) {
                if (enemy->IsAlliedWithFaction(factionObject))
                    return !RestrictToFactionIsReversed;
                return RestrictToFactionIsReversed;
            }
            return false;
        }
        return false;
    }
    return false;
}

int main()
{
    int bad = 0;
    CTCEnemy enemy; enemy.m_ally = &g_good;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0x49, (CTCBase*)&enemy } };
    CThingAICreature creature; for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[2] = 1UL << 9; creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CThingAICreature rock; for (int i = 0; i < 9; ++i) rock.m_presentBits[i] = 0; rock.m_interfaces.first = rock.m_interfaces.last = 0;
    CFactionDef good; good.m_name = 5; CFactionDef nobody; nobody.m_name = 9;
    CTCOpinionSource s; s.RestrictToFactionAndAllies.Object = 0; s.RestrictToFactionIsReversed = false;
    if (s.AppliesToWitness(0)) bad |= 1;
    if (!s.AppliesToWitness(&rock)) bad |= 2;                  // unrestricted
    s.RestrictToFactionAndAllies.Object = &good;
    if (s.AppliesToWitness(&rock)) bad |= 4;                   // no enemy component
    if (!s.AppliesToWitness(&creature)) bad |= 8;              // allied
    s.RestrictToFactionIsReversed = true;
    if (s.AppliesToWitness(&creature)) bad |= 16;
    enemy.m_ally = 0;
    if (!s.AppliesToWitness(&creature)) bad |= 32;             // not allied, reversed
    s.RestrictToFactionAndAllies.Object = &nobody;
    if (s.AppliesToWitness(&creature)) bad |= 64;              // unknown faction
    if (g_strings != 0) bad |= 128;
    if (bad) { std::printf("00771540_TEST FAIL\n"); return 1; }
    std::printf("00771540_TEST PASS\n");
    return 0;
}