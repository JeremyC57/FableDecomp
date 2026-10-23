// Behaviour test for 00956b00: body copied from src/00956b00.cpp (CAIStateGroup_BriarRose::IsRunnable), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
static CWorld* g_world = 0; static int g_getPWorld = 0;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual void v13() {}
    virtual long GetTimeToInitiate() { return 0; }
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld() { ++g_getPWorld; return g_world; }
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
static void MakeThing(CThing& t, CInterfaceMap::SEntry* entries, long type, CTCBase* component)
{
    memset(&t, 0, sizeof(t));
    entries[0].type = type; entries[0].component = component; entries[1].type = 0x100;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    t.m_presentBits[type >> 5] |= 1UL << (type & 31);
}
struct CWorld { int dummy; };
static CThing* g_setItem = 0; static int g_setCalls = 0;
struct CBaseIntelligentPointerCtor {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointerCtor() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { ++g_setCalls; g_setItem = item; m_pItem = item; }
    ~CBaseIntelligentPointerCtor() {}
};
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    CIntelligentPointerThing() {}
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
static CThing* g_enemy = 0; static int g_getEnemy = 0;
struct CTCEnemySource {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {}
    virtual CThing* GetEnemy() { ++g_getEnemy; return g_enemy; }
};
static bool g_ready = false; static int g_readyCalls = 0;
struct CTCBoss { bool IsReadyForPhaseChange() { ++g_readyCalls; return g_ready; } };
static long g_frame = 0; static int g_frameCalls = 0;
struct CWorldFrame { static long GetWorldFrame() { ++g_frameCalls; return g_frame; } };
struct CAIStateGroup_BriarRose : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;
    long NextRunFrame;
    virtual bool IsRunnable();
};
bool CAIStateGroup_BriarRose::IsRunnable()
{
    CThing* creature = PBrain->PCreature;
    CTCEnemySource* source;
    creature->GetInterface(&source, 8);
    CTCBoss* boss;
    if (!(creature->GetInterface(&boss, 0xad) && boss->IsReadyForPhaseChange())) {
        if (((CWorldFrame*)GetPWorld())->GetWorldFrame() < NextRunFrame)
            return false;
    }
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        PEnemy = enemy;
        return true;
    }
    return false;
}
static void Reset() { g_setCalls = 0; g_getEnemy = 0; g_readyCalls = 0; g_frameCalls = 0; g_getPWorld = 0; }
int main()
{
    static CThing creature, enemy; static CInterfaceMap::SEntry entries[3]; static CTCEnemySource source; static CTCBoss boss; static CAIBrain brain; static CWorld world;
    memset(&creature, 0, sizeof(creature)); memset(&enemy, 0, sizeof(enemy)); g_world = &world;
    entries[0].type = 8; entries[0].component = (CTCBase*)&source; entries[1].type = 0xad; entries[1].component = (CTCBase*)&boss; entries[2].type = 0x100;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 3;
    creature.m_presentBits[0] |= 1UL << 8; creature.m_presentBits[0xad >> 5] |= 1UL << (0xad & 31);
    brain.PCreature = &creature;
    CAIStateGroup_BriarRose sg; sg.PBrain = &brain; sg.NextRunFrame = 500;
    // 1: phase ready, live enemy -> runnable, enemy remembered, frame never asked
    Reset(); g_ready = true; g_enemy = &enemy;
    CHECK(sg.IsRunnable()); CHECK(g_readyCalls == 1 && g_frameCalls == 0 && g_getEnemy == 1); CHECK(g_setCalls == 2 && g_setItem == &enemy && sg.PEnemy.m_pItem == &enemy);
    // 2: not ready and the frame has not come
    Reset(); g_ready = false; g_frame = 499; sg.PEnemy.m_pItem = 0;
    CHECK(!sg.IsRunnable()); CHECK(g_frameCalls == 1 && g_getPWorld == 1 && g_getEnemy == 0 && g_setCalls == 0);
    // 3: not ready but the frame has come
    Reset(); g_frame = 500;
    CHECK(sg.IsRunnable()); CHECK(g_frameCalls == 1 && g_getEnemy == 1 && g_setCalls == 2);
    // 4: dead enemy / no enemy
    Reset(); enemy.m_flags91 = 1; CHECK(!sg.IsRunnable()); CHECK(g_setCalls == 0);
    Reset(); g_enemy = 0; CHECK(!sg.IsRunnable()); CHECK(g_setCalls == 0);
    // 5: no boss component: the frame test decides
    Reset(); creature.m_presentBits[0xad >> 5] = 0; g_enemy = &enemy; enemy.m_flags91 = 0; g_frame = 10;
    CHECK(!sg.IsRunnable()); CHECK(g_readyCalls == 0 && g_frameCalls == 1);
    if (fails == 0) printf("00956b00_TEST PASS\n"); else printf("00956b00_TEST FAIL\n");
    return fails;
}