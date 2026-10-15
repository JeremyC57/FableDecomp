// Behaviour test for 0x008BA0B0 CCreatureAction_ForcePushOutOf::PerformAction: a "CTCForcePushPower" component is
// added (initialise=true, no params); IgnoreFriendlies is set only when OnlyHitEnemies; mode 0x5b is removed when a
// mode manager (0x31) exists; the name string is destroyed before the flag store.
#include <cstring>
#include <cstdio>

struct CTCBase { int calls; void* arg; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    int m_killed;
    void Kill(bool removeNow) { m_killed = removeNow ? 2 : 1; m_flags91 |= 1; }
    template <class T> void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
    void Init()
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = 0; m_flags91 = 0; m_killed = 0;
    }
    void AddTC(CInterfaceMap::SEntry* entries, int n, long type)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        m_interfaces.first = entries; m_interfaces.last = entries + n;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static int g_liveStrings = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* t, long n = -1) : m_pData(t) { (void)n; ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};
struct CTCForcePushPower : CTCBase { bool IgnoreFriendlies; };
static CTCForcePushPower g_power;
static int g_addCalls = 0;
static CTCBase* AddTC(CThing*, const CCharString& name, bool init, void* params)
{
    if (std::strcmp(name.m_pData, "CTCForcePushPower") == 0 && init && params == 0) ++g_addCalls;
    g_power.IgnoreFriendlies = false;
    return &g_power;
}
struct CTCCreatureModeManager : CTCBase { void RemoveMode(unsigned long m) { ++calls; arg = (void*)m; } };
struct CCreatureAction_ForcePushOutOf {
    CBaseIntelligentPointer PCreature; bool OnlyHitEnemies; int m_stringsAfterAdd;
    void PerformAction()
    {
        CTCForcePushPower* power;
        {
            CCharString name("CTCForcePushPower", -1);
            power = (CTCForcePushPower*)AddTC(PCreature.GetPItem(), name, true, 0);
        }
        m_stringsAfterAdd = g_liveStrings;
        if (OnlyHitEnemies)
            power->IgnoreFriendlies = true;
        CThing* c = PCreature.GetPItem();
        if (c->m_presentBits[0x31 >> 5] & (1UL << (0x31 & 31))) {
            CTCCreatureModeManager* modes; c->GetInterface(&modes, 0x31);
            modes->RemoveMode(0x5b);
        }
    }
};
int main()
{
    CTCCreatureModeManager modes; modes.calls = 0; modes.arg = 0;
    CInterfaceMap::SEntry e[1] = { { 0x31, &modes } };
    CThing creature; creature.Init();
    CCreatureAction_ForcePushOutOf a; a.PCreature.m_pItem = &creature; a.OnlyHitEnemies = false;
    a.PerformAction();
    bool ok = g_addCalls == 1 && !g_power.IgnoreFriendlies && modes.calls == 0 && a.m_stringsAfterAdd == 0;
    creature.AddTC(e, 1, 0x31); a.OnlyHitEnemies = true; a.PerformAction();
    ok = ok && g_addCalls == 2 && g_power.IgnoreFriendlies && modes.calls == 1 && modes.arg == (void*)0x5b;
    if (!ok) { std::printf("008ba0b0_TEST FAIL\n"); return 1; }
    std::printf("008ba0b0_TEST PASS\n");
    return 0;
}
