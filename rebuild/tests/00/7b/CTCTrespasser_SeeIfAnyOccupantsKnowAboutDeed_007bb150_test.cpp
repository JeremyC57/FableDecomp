// Behaviour test for 007bb150: body copied from src/007bb150.cpp (CTCTrespasser::SeeIfAnyOccupantsKnowAboutDeed), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
typedef long EOpinionDeedType;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    virtual void v00() {}
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CBaseIntelligentPointer { void* _vfptr; CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static int g_knows = 0; static long g_knowsHandle = -1; static EOpinionDeedType g_knowsType = -1; static bool g_knowsIncl = false; static void* g_knowsThis = 0;
class CTCCreatureOpinionOfHero { public:
    bool m_knows;
    bool KnowsOfDeed(long deedHandle, EOpinionDeedType type, bool includeReported) { ++g_knows; g_knowsHandle = deedHandle; g_knowsType = type; g_knowsIncl = includeReported; g_knowsThis = this; return m_knows; }
};
class CWorld { public: static unsigned long GetWorldFrame() { return 777; } };
struct SOccupantNode { SOccupantNode* _M_next; SOccupantNode* _M_prev; CBaseIntelligentPointer _M_data; };
#pragma pack(push, 1)
class CTCTrespasser { public:
    virtual void v00() {}
    CThing*       ParentThing;
    unsigned char _pad_0x08[0x4];
    long          InsideBuildingNotTrespassingDeedHandle;
    long          DeedHandles[3];
    long          CurrentDeedHandleIndex;
    long          OccupantsKnowOfDeedHandleIndex;
    unsigned long FrameSomeoneWasInformedOfLastDeed;
    unsigned long FramePlayerLeft;
    bool          AttachedOpinionSource;
    CBaseIntelligentPointer TrespassingThing;
    unsigned char _pad_0x35[0x3];
    SOccupantNode* OccupantsOfCurrentTrespassingThing;
    bool SeeIfAnyOccupantsKnowAboutDeed(long deedIndex, EOpinionDeedType type);
};
#pragma pack(pop)
bool CTCTrespasser::SeeIfAnyOccupantsKnowAboutDeed(long deedIndex, EOpinionDeedType type)
{
    SOccupantNode* end = OccupantsOfCurrentTrespassingThing;
    for (SOccupantNode* node = end->_M_next; node != end; node = node->_M_next) {
        CThing* occupant = node->_M_data.GetPItem();
        if (occupant && occupant->HasTC(79)) {
            CTCCreatureOpinionOfHero* opinion = occupant->GetTC<CTCCreatureOpinionOfHero>(79);
            if (opinion->KnowsOfDeed(DeedHandles[deedIndex], type, true)) {
                OccupantsKnowOfDeedHandleIndex = deedIndex;
                FrameSomeoneWasInformedOfLastDeed = CWorld::GetWorldFrame();
                return true;
            }
        }
    }
    return false;
}
int main()
{
    CHECK(sizeof(CTCTrespasser) == 0x3c);
    CThing a, b; memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); CTCCreatureOpinionOfHero opA, opB; opA.m_knows = false; opB.m_knows = false;
    CInterfaceMap::SEntry ea[2] = { { 79, (CTCBase*)&opA }, { 90, (CTCBase*)0x99 } }; a.m_interfaces.first = ea; a.m_interfaces.last = ea + 2;
    CInterfaceMap::SEntry eb[2] = { { 79, (CTCBase*)&opB }, { 90, (CTCBase*)0x99 } }; b.m_interfaces.first = eb; b.m_interfaces.last = eb + 2;
    SOccupantNode head, n1, n2, n3; head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data.m_pItem = 0; n2._M_data.m_pItem = &a; n3._M_data.m_pItem = &b;
    CTCTrespasser t; memset(&t, 0, sizeof(t)); t.OccupantsOfCurrentTrespassingThing = &head; t.DeedHandles[0] = 100; t.DeedHandles[1] = 200; t.DeedHandles[2] = 300;
    t.OccupantsKnowOfDeedHandleIndex = -1;
    // 1. nobody has the opinion component: false, no query
    CHECK(t.SeeIfAnyOccupantsKnowAboutDeed(1, 5) == false && g_knows == 0 && t.OccupantsKnowOfDeedHandleIndex == -1);
    // 2. both have it, neither knows: two queries, false
    a.m_presentBits[79 >> 5] = 1UL << (79 & 31); b.m_presentBits[79 >> 5] = 1UL << (79 & 31);
    CHECK(t.SeeIfAnyOccupantsKnowAboutDeed(1, 5) == false && g_knows == 2 && g_knowsHandle == 200 && g_knowsType == 5 && g_knowsIncl == true && g_knowsThis == &opB);
    // 3. the second knows: index and frame recorded, true
    opB.m_knows = true;
    CHECK(t.SeeIfAnyOccupantsKnowAboutDeed(2, 7) == true && g_knows == 4 && g_knowsHandle == 300 && t.OccupantsKnowOfDeedHandleIndex == 2 && t.FrameSomeoneWasInformedOfLastDeed == 777);
    // 4. the first knows: stops after one query
    opA.m_knows = true; CHECK(t.SeeIfAnyOccupantsKnowAboutDeed(0, 1) == true && g_knows == 5 && g_knowsThis == &opA && t.OccupantsKnowOfDeedHandleIndex == 0);
    // 5. empty list
    head._M_next = &head; CHECK(t.SeeIfAnyOccupantsKnowAboutDeed(0, 1) == false && g_knows == 5);
    if (fails == 0) printf("007bb150_TEST PASS\n"); else printf("007bb150_TEST FAIL\n");
    return fails;
}