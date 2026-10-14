#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CAIStateGroupBase;
static bool g_preferred;
struct CTCIdleScheduler { bool IsPreferredIdle(CAIStateGroupBase*) { Log("ask"); return g_preferred; } };
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

struct CAIBrain { unsigned char _pad[0x18]; CThingAICreature* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_Clean : CAIStateGroupBase { virtual bool IsRunnable() { Log("clean"); return true; } };
struct CAIStateGroup_CleanHouse : CAIStateGroup_Clean {
    virtual bool IsRunnable() { CTCIdleScheduler* s; if (PBrain->Creature->GetInterface(0xd3, &s)) { if (!s->IsPreferredIdle(this)) return false; } return CAIStateGroup_Clean::IsRunnable(); }
};
int main()
{
    CThingAICreature c; CAIBrain b; b.Creature = &c; CInterfaceMap::SEntry table[2]; CTCIdleScheduler sched; CAIStateGroup_CleanHouse g; g.PBrain = &b;
    c.Clear(); if (!g.IsRunnable()) return Check("00917b50", "x");
    c.Set(0xd3, &sched, table); g_preferred = false; if (g.IsRunnable()) return Check("00917b50", "x");
    g_preferred = true; if (!g.IsRunnable()) return Check("00917b50", "x");
    return Check("00917b50", "clean ask ask clean ");
}