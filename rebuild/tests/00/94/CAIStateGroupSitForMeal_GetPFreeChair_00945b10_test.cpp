// Behaviour test for 00945b10: body copied from src/00945b10.cpp (CAIStateGroup_SitForMeal::GetPFreeChair), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase { };
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
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
static int g_getPItem = 0;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const { ++g_getPItem; return m_pItem; }
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
struct SThingListNode { SThingListNode* next; SThingListNode* prev; CBaseIntelligentPointer value; };
struct SThingListIterator {
    SThingListNode* node;
    SThingListIterator() {}
    SThingListIterator(SThingListNode* n) : node(n) {}
};
struct CThingList {
    unsigned char allocator;
    SThingListNode* head;
    long size;
    SThingListIterator begin() const { return SThingListIterator(head->next); }
    SThingListIterator end() const { return SThingListIterator(head); }
};
struct CTCBuildingContents : CTCBase { unsigned char _pad[0x8]; CThingList Things; };
struct CThingAICreature : CThing {
    unsigned char _pad2[0x168 - sizeof(CThing)];
    CBaseIntelligentPointer PHome;
    CThing* GetPHome() const { return PHome.GetPItem(); }
};
struct CAIBrainSitForMeal { unsigned char _pad[0x18]; CThingAICreature* PCreature; };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    CAIBrainSitForMeal* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    bool Terminated;
    unsigned char Priority;
    bool Enabled;
};
static SThingListNode* g_findFirst = 0; static SThingListNode* g_findLast = 0; static SThingListNode* g_findResult = 0; static int g_findCalls = 0;
static CThing* g_predBuilding = 0;
struct CIsFreeChairInBuilding {
    CThing* PBuilding;
    CIsFreeChairInBuilding(CThing* building) : PBuilding(building) {}
};
SThingListIterator __fastcall FindIfFreeChair(SThingListIterator first, SThingListIterator last, CIsFreeChairInBuilding pred)
{
    ++g_findCalls; g_findFirst = first.node; g_findLast = last.node; g_predBuilding = pred.PBuilding;
    return SThingListIterator(g_findResult ? g_findResult : last.node);
}
struct CThingChair { unsigned char _pad[0x6c]; unsigned long ThingFlags; };
struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];
    CBaseIntelligentPointer PPot;
    long ActionPointIndex;
    bool HoldingBowl;
    long DayLastAte;
    CThing* GetPFreeChair();
};
CThing* CAIStateGroup_SitForMeal::GetPFreeChair()
{
    CThing* home = PBrain->PCreature->GetPHome();
    CTCBuildingContents* contents;
    if (home == 0 || !home->GetInterface(&contents, 0x21))
        return 0;
    {
        SThingListIterator it = FindIfFreeChair(contents->Things.begin(), contents->Things.end(),
                                                CIsFreeChairInBuilding(home));
        if (it.node != contents->Things.end().node) {
            CThingChair* chair = (CThingChair*)it.node->value.GetPItem();
            if (chair->ThingFlags & 0x800)
                return (CThing*)chair;
        }
    }
    return 0;
}
int main()
{
    static CThingAICreature creature; static CThing home; static CTCBuildingContents contents; static CInterfaceMap::SEntry entries[2];
    static CAIBrainSitForMeal brain; static SThingListNode head, n1; static CThingChair chair;
    memset(&home, 0, sizeof(home)); memset(&creature, 0, sizeof(creature)); memset(&chair, 0, sizeof(chair));
    entries[0].type = 0x05; entries[1].type = 0x21; entries[1].component = &contents;
    home.m_interfaces.first = entries; home.m_interfaces.last = entries + 2; home.m_presentBits[1] |= 1UL << (0x21 & 31);
    contents.Things.head = &head; head.next = &n1; n1.next = &head; n1.value.m_pItem = (CThing*)&chair;
    brain.PCreature = &creature; creature.PHome.m_pItem = &home;
    CAIStateGroup_SitForMeal sg; sg.PBrain = &brain;
    // 1: a usable chair found
    g_findCalls = 0; g_findResult = &n1; chair.ThingFlags = 0x800;
    CHECK(sg.GetPFreeChair() == (CThing*)&chair); CHECK(g_findCalls == 1); CHECK(g_findFirst == &n1 && g_findLast == &head); CHECK(g_predBuilding == &home);
    // 2: found but not a usable seat
    chair.ThingFlags = 0x7ff; CHECK(sg.GetPFreeChair() == 0);
    // 3: nothing found
    g_findResult = 0; chair.ThingFlags = 0x800; g_findCalls = 0; CHECK(sg.GetPFreeChair() == 0); CHECK(g_findCalls == 1);
    // 4: home without the contents component
    home.m_presentBits[1] = 0; g_findCalls = 0; CHECK(sg.GetPFreeChair() == 0); CHECK(g_findCalls == 0);
    // 5: no home
    creature.PHome.m_pItem = 0; CHECK(sg.GetPFreeChair() == 0); CHECK(g_findCalls == 0);
    if (fails == 0) printf("00945b10_TEST PASS\n"); else printf("00945b10_TEST FAIL\n");
    return fails;
}