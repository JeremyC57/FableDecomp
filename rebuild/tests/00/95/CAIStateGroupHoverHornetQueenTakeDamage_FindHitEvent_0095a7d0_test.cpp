// Behaviour test for 0095a7d0: body copied from src/0095a7d0.cpp (CAIStateGroup_HoverHornetQueenTakeDamage::FindHitEvent), callees stubbed.
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
namespace NEntityEvents {
    class CEventHitBy;
    struct CIsHitEventHitByRanged { CIsHitEventHitByRanged() { } };
    class CIsHitEventHitByMelee { public: CIsHitEventHitByMelee() { } };
    class CIsEventCreatedAfter { public:
        long m_frame;
        CIsEventCreatedAfter(long frame) : m_frame(frame) { }
    };
}
using namespace NEntityEvents;
typedef CFilter_Bind2<CIsHitEventHitByRanged, CIsEventCreatedAfter> CRangedHitFilter;
typedef CFilter_Bind2<CIsHitEventHitByMelee, CIsEventCreatedAfter> CMeleeHitFilter;
static bool g_ranged = false, g_melee = false; static int g_rangedCalls = 0, g_meleeCalls = 0; static long g_rangedFrame = 0, g_meleeFrame = 0;
class CTCEntityEvents { public:
    bool GetEvent(const CEventHitBy** ppEvent, const CRangedHitFilter& filter) { ++g_rangedCalls; g_rangedFrame = filter.m_pSecond->m_frame; return g_ranged; }
    bool GetEvent(const CEventHitBy** ppEvent, const CMeleeHitFilter& filter) { ++g_meleeCalls; g_meleeFrame = filter.m_pSecond->m_frame; return g_melee; }
};
struct CAIStateGroup_HoverHornetQueenTakeDamage : CAIStateGroupBase {
    unsigned char _pad14[8];
    bool LashOut;
    bool bIsRangeAttack;
    unsigned char _pad1e[2];
    long RangeAttackCount;
    long FrameOfLastHit;
    bool FindHitEvent();
};
bool CAIStateGroup_HoverHornetQueenTakeDamage::FindHitEvent()
{
    bIsRangeAttack = false;
    CTCEntityEvents* events;
    if (PBrain->PCreature->GetInterface(&events, 0x42)) {
        const CEventHitBy* hit;
        if (events->GetEvent(&hit, CIsHitEventHitByRanged() && CIsEventCreatedAfter(FrameOfLastHit + 2)))
            return bIsRangeAttack = true;
        if (events->GetEvent(&hit, CIsHitEventHitByMelee() && CIsEventCreatedAfter(FrameOfLastHit + 2)))
            return true;
    }
    return false;
}
static void Reset() { g_rangedCalls = g_meleeCalls = 0; g_rangedFrame = g_meleeFrame = 0; }
int main()
{
    static CThing creature; static CInterfaceMap::SEntry entries[2]; static CTCEntityEvents events; static CAIBrain brain;
    MakeThing(creature, entries, 0x42, (CTCBase*)&events); brain.PCreature = &creature;
    CAIStateGroup_HoverHornetQueenTakeDamage sg; sg.PBrain = &brain; sg.FrameOfLastHit = 100;
    // 1: ranged hit found
    Reset(); sg.bIsRangeAttack = false; g_ranged = true; g_melee = true;
    CHECK(sg.FindHitEvent()); CHECK(sg.bIsRangeAttack); CHECK(g_rangedCalls == 1 && g_meleeCalls == 0); CHECK(g_rangedFrame == 102);
    // 2: melee hit only
    Reset(); sg.bIsRangeAttack = true; g_ranged = false;
    CHECK(sg.FindHitEvent()); CHECK(!sg.bIsRangeAttack); CHECK(g_rangedCalls == 1 && g_meleeCalls == 1); CHECK(g_meleeFrame == 102);
    // 3: no hit
    Reset(); sg.bIsRangeAttack = true; g_melee = false;
    CHECK(!sg.FindHitEvent()); CHECK(!sg.bIsRangeAttack); CHECK(g_rangedCalls == 1 && g_meleeCalls == 1);
    // 4: no events component
    Reset(); sg.bIsRangeAttack = true; creature.m_presentBits[0x42 >> 5] = 0; g_ranged = g_melee = true;
    CHECK(!sg.FindHitEvent()); CHECK(!sg.bIsRangeAttack); CHECK(g_rangedCalls == 0 && g_meleeCalls == 0);
    if (fails == 0) printf("0095a7d0_TEST PASS\n"); else printf("0095a7d0_TEST FAIL\n");
    return fails;
}