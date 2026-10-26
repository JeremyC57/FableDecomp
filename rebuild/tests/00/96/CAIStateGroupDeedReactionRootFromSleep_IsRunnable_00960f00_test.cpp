// Behaviour test for 00960f00: body copied from src/00960f00.cpp (CAIStateGroup_DeedReactionRootFromSleep::IsRunnable),
// LowerBound and the base IsRunnable stubbed so the lookup and the base call are observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
static int g_lbCalls = 0; static long g_lbKey = 0;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    ++g_lbCalls; g_lbKey = *type;
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
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
class CTCSleep { public: unsigned char _pad[0xc]; bool m_bAsleep; };
class CAIBrain { public: unsigned char _pad[0x18]; CThing* Creature; };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData;
    unsigned char Terminated, Priority, Enabled, _pad_0x13;
};
static int g_baseCalls = 0; static bool g_baseResult = false;
struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase {
    unsigned char ThingReactingTo[8];
    long CurrentDeedType;
    virtual bool IsRunnable() { ++g_baseCalls; return g_baseResult; }
};
struct CAIStateGroup_DeedReactionRootFromSleep : CAIStateGroup_DeedReactionRoot {
    virtual bool IsRunnable();
};
bool CAIStateGroup_DeedReactionRootFromSleep::IsRunnable()
{
    CTCSleep* sleep;
    bool asleep = false;
    if (PBrain->Creature->GetInterface(&sleep, 0xD4))
        asleep = sleep->m_bAsleep;
    if (!asleep)
        return false;
    if (CAIStateGroup_DeedReactionRoot::IsRunnable()) {
        long type = CurrentDeedType;
        if ((type != 0 && type < 10) || type == 0x35 || type == 10 || type == 11)
            sleep->m_bAsleep = false;
    }
    return false;
}
static bool run(bool hasSleep, bool asleep, bool base, long deed, bool* asleepAfter, int* baseCalls, int* lbCalls)
{
    CTCSleep sleep; memset(&sleep, 0, sizeof(sleep)); sleep.m_bAsleep = asleep;
    CInterfaceMap::SEntry entries[3] = { { 5, 0 }, { 0xD4, (CTCBase*)&sleep }, { 0x100, 0 } };
    CThing thing; memset(&thing, 0, sizeof(thing));
    if (hasSleep) thing.m_presentBits[6] = 0x100000;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CAIBrain brain; memset(&brain, 0, sizeof(brain)); brain.Creature = &thing;
    CAIStateGroup_DeedReactionRootFromSleep g; g.PBrain = &brain; g.CurrentDeedType = deed;
    g_baseCalls = 0; g_lbCalls = 0; g_baseResult = base;
    bool r = g.IsRunnable();
    *asleepAfter = sleep.m_bAsleep; *baseCalls = g_baseCalls; *lbCalls = g_lbCalls;
    return r;
}
int main()
{
    bool after; int bc, lc;
    CHECK(!run(false, true, true, 3, &after, &bc, &lc)); CHECK(after); CHECK(bc == 0); CHECK(lc == 0);
    CHECK(!run(true, false, true, 3, &after, &bc, &lc)); CHECK(!after); CHECK(bc == 0); CHECK(lc == 1); CHECK(g_lbKey == 0xD4);
    CHECK(!run(true, true, false, 3, &after, &bc, &lc)); CHECK(after); CHECK(bc == 1);
    long wake[] = { 1, 9, 0x35, 10, 11 };
    CHECK(!run(true, true, true, -1, &after, &bc, &lc)); CHECK(!after);
    for (int i = 0; i < 5; ++i) { CHECK(!run(true, true, true, wake[i], &after, &bc, &lc)); CHECK(!after); CHECK(bc == 1); }
    long keep[] = { 0, 12, 0x34, 0x36, 13 };
    for (int i = 0; i < 5; ++i) { CHECK(!run(true, true, true, keep[i], &after, &bc, &lc)); CHECK(after); CHECK(bc == 1); }
    if (fails == 0) printf("00960f00_TEST PASS\n"); else printf("00960f00_TEST FAIL\n");
    return fails;
}