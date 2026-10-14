#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CAIBrain {}; struct CAIStateGroupBase;
struct CTCIdleScheduler { void RegisterIdleStateGroup(CAIStateGroupBase*, long t) { Log(t == 2 ? "register2" : "register?"); } };
struct CInterfaceMap {
    struct SEntry { long type; CTCIdleScheduler* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThingAICreature {
    unsigned char _pad0[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces;
    bool GetInterface(long type, CTCIdleScheduler** out)
    {
        if (m_presentBits[type / 32] & (1 << (type % 32))) {
            long key = type; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type) it = map->last;
            *out = it->component; return true;
        }
        return false;
    }
    void Set(long type, CTCIdleScheduler* c, CInterfaceMap::SEntry* table) { m_presentBits[type / 32] |= 1 << (type % 32); table[0].type = 0x10; table[0].component = 0; table[1].type = type; table[1].component = c; m_interfaces.first = table; m_interfaces.last = table + 2; }
    void Clear() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = 0; }
};

struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; };
struct CAIStateGroup_CleanHouse : CAIStateGroupBase {
    bool m_startedCleaning; bool m_finishedCleaning; unsigned char _pad2[0xa]; long m_cleanCount; bool m_registered;
    virtual void InitStateGroup(CAIBrain&, CThingAICreature& creature)
    { m_registered = false; m_cleanCount = 0; m_startedCleaning = false; m_finishedCleaning = false; CTCIdleScheduler* s; if (creature.GetInterface(0xd3, &s)) s->RegisterIdleStateGroup(this, 2); }
};
int main()
{
    CAIBrain b; CThingAICreature c; CInterfaceMap::SEntry table[2]; CTCIdleScheduler sched; CAIStateGroup_CleanHouse g;
    g.m_registered = true; g.m_cleanCount = 9; g.m_startedCleaning = true; g.m_finishedCleaning = true;
    c.Clear(); g.InitStateGroup(b, c);
    if (g.m_registered || g.m_cleanCount != 0 || g.m_startedCleaning || g.m_finishedCleaning || g_n != 0) return Check("00917af0", "x");
    c.Set(0xd3, &sched, table); g.InitStateGroup(b, c);
    return Check("00917af0", "register2 ");
}
