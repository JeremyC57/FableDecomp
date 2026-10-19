// Behaviour test for 0x008CF610 CCreatureAction_TentacleRise::FinishAction: runs the reconstructed body over stubbed callees;
// checks RemoveMode(0x54), the interface-9 flag, the base call and the TentacleIdle(owner, code 50) action handed over.

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
struct CTCInterface9 { bool m_flag10; };
struct CCreatureAction_TentacleIdle : CCombatActionBase {
    virtual int Kind() const { return 1; }
    CCreatureAction_TentacleIdle(CThingCreatureBase* owner) : CCombatActionBase(owner, 50, CSoundPair()) { }
};
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase& a) { ++g_setCalls; g_setCode = a.m_code; g_setVt = a.Kind(); return a.PCreature.m_pItem == this; }
struct CCreatureAction_TentacleRise : CCombatActionBase {
    CTCInterface9* GetInterface9() const { CTCInterface9* c = 0; PCreature.GetPItem()->GetInterface(&c, 9); return c; }
    void FinishAction();
};
void CCreatureAction_TentacleRise::FinishAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x54);
    GetInterface9()->m_flag10 = true;
    CCombatActionBase::FinishAction();
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleIdle(PCreature.GetPItem()));
}
int main()
{
    CTCCreatureModeManager mm; CTCInterface9 c9; c9.m_flag10 = false;
    CThingCreatureBase owner; owner = CThingCreatureBase(); owner.Add(9, &c9); owner.Add(0x31, &mm);
    CCreatureAction_TentacleRise a; a.PCreature.m_pItem = &owner;
    a.FinishAction();
    bool ok = g_modeCalls == 1 && g_modeArg == 0x54 && c9.m_flag10 && g_base == 1 && g_setCalls == 1 && g_setCode == 50 && g_setVt == 1 && g_dtors == 1;
    if (!ok) { std::printf("008cf610_TEST FAIL\n"); return 1; }
    std::printf("008cf610_TEST PASS\n");
    return 0;
}
