// Behaviour test for 00972340: body copied from src/00972340.cpp (CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush), callees stubbed.
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
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
static int g_destroyed = 0;
struct CAICreatureWillPowerIndicatorDef {
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    unsigned char _pad[0x38];
    long HitsToRush;
};
static CAICreatureWillPowerIndicatorDef* g_def = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef(CAICreatureWillPowerIndicatorDef** out) { ++g_getSubDef; ++g_def->m_refCount; *out = g_def; return true; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad4c[0x24];
    CDefClassBase* PDef;
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
struct CTCBoss {
    unsigned char _pad[0x10];
    long m_hitCount;
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain;
    unsigned char _pad[0x0c];
};
struct CAIStateGroup_JackOfBladesStage1 : CAIStateGroupBase {
    bool HasHitsCausedRush();
};
bool CAIStateGroup_JackOfBladesStage1::HasHitsCausedRush()
{
    CThing* creature = PBrain->PCreature;
    CTCBoss* boss;
    creature->GetInterface(&boss, 0xad);
    CAICreatureWillPowerIndicatorDef* subDef = 0;
    creature->PDef->GetSubDef(&subDef);
    CAICreatureWillPowerIndicatorDef* def = subDef;
    if (def->HitsToRush == 0) {
        if (--def->m_refCount == 0)
            def->Destroy();
        return false;
    }
    bool rushed = boss->m_hitCount == def->HitsToRush;
    if (--def->m_refCount == 0)
        def->Destroy();
    return rushed;
}
int main()
{
    static CThing creature; static CAIBrain brain; static CTCBoss boss; static CDefClassBase classDef; static CAICreatureWillPowerIndicatorDef def;
    memset(&creature, 0, sizeof(creature)); creature.PDef = &classDef; brain.PCreature = &creature; def.m_refCount = 1; g_def = &def;
    CInterfaceMap::SEntry entries[3] = { { 0x10, 0 }, { 0xad, (CTCBase*)&boss }, { 0xc0, 0 } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3; creature.m_presentBits[0xad >> 5] |= 1UL << (0xad & 31);
    CAIStateGroup_JackOfBladesStage1 g; g.PBrain = &brain;
    // a zero threshold never rushes
    def.HitsToRush = 0; boss.m_hitCount = 0; CHECK(!g.HasHitsCausedRush()); CHECK(g_getSubDef == 1 && def.m_refCount == 1 && g_destroyed == 0);
    // threshold 3: rushes exactly when the hit count reaches it
    def.HitsToRush = 3; boss.m_hitCount = 2; CHECK(!g.HasHitsCausedRush()); boss.m_hitCount = 3; CHECK(g.HasHitsCausedRush()); boss.m_hitCount = 4; CHECK(!g.HasHitsCausedRush());
    CHECK(def.m_refCount == 1 && g_destroyed == 0);
    // a def held only by the lookup is destroyed on both exits
    def.m_refCount = 0; boss.m_hitCount = 3; CHECK(g.HasHitsCausedRush()); CHECK(g_destroyed == 1);
    def.m_refCount = 0; def.HitsToRush = 0; CHECK(!g.HasHitsCausedRush()); CHECK(g_destroyed == 2);
    if (fails == 0) printf("00972340_TEST PASS\n"); else printf("00972340_TEST FAIL\n");
    return fails;
}