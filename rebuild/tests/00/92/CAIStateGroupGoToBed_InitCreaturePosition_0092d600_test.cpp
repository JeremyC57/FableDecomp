// Behaviour test for 0092d600: body copied from src/0092d600.cpp (CAIStateGroup_GoToBed::InitCreaturePosition), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;
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
class CThing { public:
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
class CAIBrain { public: unsigned char _pad[0x18]; CThing* PCreature; };
static CWorld* g_world = 0; static int g_getPWorld = 0;
class CAIStateGroupBase { public:
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
template<class A, class B> class CFilter_Bind2 { public:
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
static bool g_inRange = false; static long g_rangeStart = 0, g_rangeEnd = 0;
struct CGameTime {
    bool IsTimeOfDayInRange(long start, long end) { g_rangeStart = start; g_rangeEnd = end; return g_inRange; }
};
class CWorld { public:
    unsigned char _pad0[0x1c];
    CGameTime* m_pTime;
};
struct CAIStateGroupBaseW : CAIStateGroupBase { };
class C3DVector { public: float x, y, z; };
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
struct CThingPositioned { const C3DVector& GetPosition() { return g_pos; } };
class CThingAICreature { public: void GetSleepTimes(long* start, long* end) { *start = 1320; *end = 360; } };
struct CTCSleeper {
    void* __vftable;
    CThingPositioned* Owner;
    unsigned char _pad08[4];
    bool m_hasWakePos;
    bool m_inBed;
    unsigned char _pad0e[2];
    long m_frameStarted;
    C3DVector m_wakePos;
};
static int g_frame = 5000000;
long __fastcall GFGetCurrentFrame() { return g_frame; }
struct CThingWorld { static long GetCurrentFrame() { return GFGetCurrentFrame(); } };
static bool g_freeBed = false, g_banditBed = false; static int g_findBed = 0, g_findBandit = 0, g_onReplace = 0; static bool g_allowOwned = false;
struct CAIStateGroup_GoToBed : CAIStateGroupBaseW {
    unsigned char PBed[8];
    long m_sleepTimeOffset;
    long m_bedTimeDelay;
    long m_wakeTimeDelay;
    CThingWorld* GetPFrameWorld() { return 0; }
    bool FindFreeBed(bool allowOwned) { ++g_findBed; g_allowOwned = allowOwned; return g_freeBed; }
    bool FindFreeBanditBed() { ++g_findBandit; return g_banditBed; }
    virtual void OnReplace() { ++g_onReplace; }
    virtual bool InitCreaturePosition();
    bool IsInSleepTime()
    {
        CGameTime* pTime = GetPWorld()->m_pTime;
        long sleepEnd;
        long sleepStart;
        ((CThingAICreature*)PBrain->PCreature)->GetSleepTimes(&sleepStart, &sleepEnd);
        return pTime->IsTimeOfDayInRange(sleepStart + m_bedTimeDelay, sleepEnd);
    }
};
bool CAIStateGroup_GoToBed::InitCreaturePosition()
{
    CTCSleeper* sleeper;
    if (PBrain->PCreature->GetInterface(&sleeper, 0xd4)) {
        sleeper->m_frameStarted = GetPFrameWorld()->GetCurrentFrame() - 1000000;
        if (IsInSleepTime()
            && (FindFreeBed(true) || FindFreeBanditBed())) {
            CAIStateGroup_GoToBed::OnReplace();
            sleeper->m_inBed = true;
            sleeper->m_wakePos = sleeper->Owner->GetPosition();
            sleeper->m_hasWakePos = true;
            return true;
        }
    }
    return false;
}
static void Reset(CTCSleeper& s) { memset(&s, 0, sizeof(s)); s.m_frameStarted = -1; g_findBed = g_findBandit = g_onReplace = 0; g_rangeStart = g_rangeEnd = 0; }
int main()
{
    static CThing creature; static CInterfaceMap::SEntry entries[2]; static CTCSleeper sleeper; static CAIBrain brain; static CWorld world; static CGameTime time;
    static CThingPositioned owner;
    MakeThing(creature, entries, 0xd4, (CTCBase*)&sleeper); brain.PCreature = &creature; world.m_pTime = &time; g_world = &world;
    CAIStateGroup_GoToBed sg; sg.PBrain = &brain; sg.m_bedTimeDelay = 30;
    // 1: sleep time, free bed
    Reset(sleeper); sleeper.Owner = &owner; g_inRange = true; g_freeBed = true;
    CHECK(sg.InitCreaturePosition()); CHECK(sleeper.m_frameStarted == 4000000); CHECK(g_rangeStart == 1350 && g_rangeEnd == 360);
    CHECK(g_findBed == 1 && g_allowOwned && g_findBandit == 0 && g_onReplace == 1); CHECK(sleeper.m_inBed && sleeper.m_hasWakePos);
    CHECK(sleeper.m_wakePos.x == 1.0f && sleeper.m_wakePos.y == 2.0f && sleeper.m_wakePos.z == 3.0f);
    // 2: no free bed but a bandit bed
    Reset(sleeper); sleeper.Owner = &owner; g_freeBed = false; g_banditBed = true;
    CHECK(sg.InitCreaturePosition()); CHECK(g_findBed == 1 && g_findBandit == 1 && g_onReplace == 1); CHECK(sleeper.m_inBed);
    // 3: no bed at all
    Reset(sleeper); g_banditBed = false;
    CHECK(!sg.InitCreaturePosition()); CHECK(g_findBed == 1 && g_findBandit == 1 && g_onReplace == 0); CHECK(!sleeper.m_inBed); CHECK(sleeper.m_frameStarted == 4000000);
    // 4: not sleep time: no bed search
    Reset(sleeper); g_inRange = false; g_freeBed = true;
    CHECK(!sg.InitCreaturePosition()); CHECK(g_findBed == 0 && g_findBandit == 0); CHECK(sleeper.m_frameStarted == 4000000);
    // 5: no sleeper component
    Reset(sleeper); creature.m_presentBits[0xd4 >> 5] = 0; g_inRange = true;
    CHECK(!sg.InitCreaturePosition()); CHECK(sleeper.m_frameStarted == -1); CHECK(g_findBed == 0);
    if (fails == 0) printf("0092d600_TEST PASS\n"); else printf("0092d600_TEST FAIL\n");
    return fails;
}