// Behaviour test for 0x008CF700 CCreatureAction_TentacleStrikeInto::FinishAction: runs the reconstructed body over stubbed callees;
// checks the base call, AddMode(0x53), QueueStrikeDamage and the StrikeLoop(owner, 150, -1, false, 0) action handed over.

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
static unsigned long g_modeArg = 0; static int g_modeCalls = 0;
struct CTCCreatureModeManager { void RemoveMode(unsigned long m) { g_modeArg = m; ++g_modeCalls; } bool AddMode(unsigned long m) { g_modeArg = m; ++g_modeCalls; return true; } };
struct CSoundPair { long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
struct CCombatActionBase;
static int g_setCalls = 0; static long g_setCode = 0; static int g_setVt = 0; static long g_lastTick = 0; static bool g_lastInt = true; static float g_lastHealth = 1;
struct CThingCreatureBase : CThing { bool SetCurrentAction(const CCombatActionBase& action); };
struct CBaseIntelligentPointer { CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
static int g_base = 0; static int g_dtors = 0;
struct CCombatActionBase {
    CBaseIntelligentPointer PCreature; long m_code; CSoundPair m_sound;
    virtual int Kind() const { return 0; }
    CCombatActionBase() : m_code(0) { }
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) : m_code(actionCode), m_sound(sound) { PCreature.m_pItem = owner; }
    ~CCombatActionBase() { ++g_dtors; }
    void FinishAction() { ++g_base; }
};
static int g_queued = 0;
struct CTCKrakenTentacle { void QueueStrikeDamage() { ++g_queued; } };
struct CCreatureAction_TentacleStrikeLoop : CCombatActionBase {
    long StartTick; bool Interrupt; float StartHealth;
    virtual int Kind() const { return 2; }
    CCreatureAction_TentacleStrikeLoop(CThingCreatureBase* owner) : CCombatActionBase(owner, 150, CSoundPair()), StartTick(-1), Interrupt(false), StartHealth(0) { }
};
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase& a)
{
    ++g_setCalls; g_setCode = a.m_code; g_setVt = a.Kind();
    const CCreatureAction_TentacleStrikeLoop& l = (const CCreatureAction_TentacleStrikeLoop&)a;
    g_lastTick = l.StartTick; g_lastInt = l.Interrupt; g_lastHealth = l.StartHealth; return true;
}
struct CCreatureAction_TentacleStrikeInto : CCombatActionBase {
    CTCKrakenTentacle* PTCTentacle;
    void FinishAction();
};
void CCreatureAction_TentacleStrikeInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->AddMode(0x53);
    PTCTentacle->QueueStrikeDamage();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleStrikeLoop(PCreature.GetPItem()));
}
int main()
{
    CTCCreatureModeManager mm; CTCKrakenTentacle t;
    CThingCreatureBase owner; owner = CThingCreatureBase(); owner.Add(0x31, &mm);
    CCreatureAction_TentacleStrikeInto a; a.PCreature.m_pItem = &owner; a.PTCTentacle = &t;
    a.FinishAction();
    bool ok = g_base == 1 && g_modeCalls == 1 && g_modeArg == 0x53 && g_queued == 1 && g_setCalls == 1 && g_setCode == 150 &&
              g_setVt == 2 && g_lastTick == -1 && !g_lastInt && g_lastHealth == 0.0f && g_dtors == 1;
    if (!ok) { std::printf("008cf700_TEST FAIL\n"); return 1; }
    std::printf("008cf700_TEST PASS\n");
    return 0;
}
