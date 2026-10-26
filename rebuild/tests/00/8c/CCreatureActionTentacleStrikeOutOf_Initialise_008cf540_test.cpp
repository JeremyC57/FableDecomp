// Behaviour test for 0x008CF540 CCreatureAction_TentacleStrikeOutOf::Initialise: runs the reconstructed body over stubbed callees;
// checks RemoveMode(0x53), the tentacle clean-up calls, the animation name and the base Initialise.

#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing { public:
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
static unsigned long g_modeArg = 0; static int g_modeCalls = 0;
class CTCCreatureModeManager { public: void RemoveMode(unsigned long m) { g_modeArg = m; ++g_modeCalls; } };
static int g_strings = 0;
class CCharString { public: const char* m_pData; CCharString(const char* t, long = -1) : m_pData(t) { ++g_strings; } ~CCharString() { --g_strings; } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static int g_removed = 0; static int g_striking = -1;
class CTCKrakenTentacle { public: void RemoveObstruction() { ++g_removed; } void SetIsStriking(bool s) { g_striking = s; } };
static char g_anim[32]; static int g_init = 0;
class CCombatActionBase { public:
    virtual void Initialise() { ++g_init; }
    CBaseIntelligentPointer PCreature;
    void SetAnimationName(const CCharString& n) { std::strcpy(g_anim, n.m_pData); }
    CTCKrakenTentacle* GetTentacle() const { CTCKrakenTentacle* t = 0; PCreature.GetPItem()->GetInterface(&t, 0xe3); return t; }
};
class CCreatureAction_TentacleStrikeOutOf : public CCombatActionBase { public: virtual void Initialise(); };
void CCreatureAction_TentacleStrikeOutOf::Initialise()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x53);
    CTCKrakenTentacle* tentacle = GetTentacle();
    tentacle->RemoveObstruction();
    tentacle->SetIsStriking(false);
    {
        CCharString name("TENTACLE_SMASH_OUT_OF", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}
int main()
{
    CTCCreatureModeManager mm; CTCKrakenTentacle t;
    CThing owner = CThing(); owner.Add(0x31, &mm); owner.Add(0xe3, &t);
    CCreatureAction_TentacleStrikeOutOf a; a.PCreature.m_pItem = &owner;
    a.Initialise();
    bool ok = g_modeCalls == 1 && g_modeArg == 0x53 && g_removed == 1 && g_striking == 0 &&
              std::strcmp(g_anim, "TENTACLE_SMASH_OUT_OF") == 0 && g_strings == 0 && g_init == 1;
    if (!ok) { std::printf("008cf540_TEST FAIL\n"); return 1; }
    std::printf("008cf540_TEST PASS\n");
    return 0;
}