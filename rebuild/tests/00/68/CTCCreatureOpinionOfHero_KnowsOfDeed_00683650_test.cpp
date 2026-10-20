// Behaviour test for 00683650: body copied from src/00683650.cpp (CTCCreatureOpinionOfHero::KnowsOfDeed(long,bool)), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct COpinionDeed { long Type; };
static long g_handleSeen = -1;
static COpinionDeed* g_deedToReturn = 0;
struct CTCHeroOpinionDeedLog {
    COpinionDeed* GetDeedFromHandle(long handle) { g_handleSeen = handle; return g_deedToReturn; }
};
struct CPlayer { CThing* hero; CThing* GetHeroThing() { return hero; } };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
static CPlayerManager g_pm;
static long g_typedHandle = -2, g_typedType = -2; static int g_typedPending = -1; static bool g_typedResult = false; static int g_typedCalls = 0;
struct CTCCreatureOpinionOfHero {
    virtual void v00() {}
    CThing* ParentThing;
    CPlayerManager* GetPlayerManager() { return &g_pm; }
    bool KnowsOfDeed(long deedHandle, long deedType, bool includePending) { ++g_typedCalls; g_typedHandle = deedHandle; g_typedType = deedType; g_typedPending = includePending; return g_typedResult; }
    bool KnowsOfDeed(long deedHandle, bool includePending);
};
static __forceinline CTCBaseInterface* GetTC(CThing* thing, long type)
{
    CInterfaceMap* map = &thing->m_interfaces;
    long key = type;
    CInterfaceMap::SEntry* it = map->LowerBound(&key);
    return ((it == map->last || type < it->type) ? map->last : it)->component;
}
static __forceinline CTCHeroOpinionDeedLog* GetDeedLog(CThing* thing)
{
    CTCBaseInterface* tc = 0;
    if (thing->HasTC(0x51))
        tc = GetTC(thing, 0x51);
    return (CTCHeroOpinionDeedLog*)tc;
}
bool CTCCreatureOpinionOfHero::KnowsOfDeed(long deedHandle, bool includePending)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero) {
        COpinionDeed* deed = GetDeedLog(hero)->GetDeedFromHandle(deedHandle);
        if (deed)
            return KnowsOfDeed(deedHandle, deed->Type, includePending);
    }
    return false;
}
int main()
{
    CThing hero; memset(&hero, 0, sizeof(hero));
    CTCHeroOpinionDeedLog log;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBaseInterface*)0x99 }, { 0x51, (CTCBaseInterface*)&log } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2;
    hero.m_presentBits[0x51 >> 5] = 1UL << (0x51 & 31);
    CTCCreatureOpinionOfHero o; o.ParentThing = 0;
    // 1. no hero: false, nothing called
    g_pm.player.hero = 0;
    CHECK(o.KnowsOfDeed(7, true) == false); CHECK(g_typedCalls == 0); CHECK(g_handleSeen == -1);
    // 2. hero but no deed for the handle: false
    g_pm.player.hero = &hero; g_deedToReturn = 0;
    CHECK(o.KnowsOfDeed(7, true) == false); CHECK(g_handleSeen == 7); CHECK(g_typedCalls == 0);
    // 3. deed found: the typed overload decides, with the deed's type
    COpinionDeed deed; deed.Type = 42; g_deedToReturn = &deed; g_typedResult = true;
    CHECK(o.KnowsOfDeed(9, false) == true); CHECK(g_typedCalls == 1); CHECK(g_typedHandle == 9); CHECK(g_typedType == 42); CHECK(g_typedPending == 0);
    g_typedResult = false;
    CHECK(o.KnowsOfDeed(9, true) == false); CHECK(g_typedCalls == 2); CHECK(g_typedPending == 1);
    if (fails == 0) printf("00683650_TEST PASS\n"); else printf("00683650_TEST FAIL\n");
    return fails;
}