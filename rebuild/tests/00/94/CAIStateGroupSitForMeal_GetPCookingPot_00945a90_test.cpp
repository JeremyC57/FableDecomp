// Behaviour test for 00945a90: body copied from src/00945a90.cpp (CAIStateGroup_SitForMeal::GetPCookingPot, first instantiation), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase { public: };
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
class CAIBrain { public: unsigned char _pad[0x18]; CThing* PCreature; };
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
class CThingAICreature : public CThing { public:
    unsigned char _pad2[0x168 - sizeof(CThing)];
    CBaseIntelligentPointer PHome;
    CThing* GetPHome() const { return PHome.GetPItem(); }
};
struct CAIBrainSitForMeal { unsigned char _pad[0x18]; CThingAICreature* PCreature; };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    CAIBrainSitForMeal* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    bool Terminated;
    unsigned char Priority;
    bool Enabled;
};
static SThingListNode* g_findFirst = 0; static SThingListNode* g_findLast = 0; static SThingListNode* g_findResult = 0; static int g_findCalls = 0;
struct CIsCookingPot { };
SThingListIterator __fastcall FindIfCookingPot(SThingListIterator first, SThingListIterator last, CIsCookingPot pred)
{
    ++g_findCalls; g_findFirst = first.node; g_findLast = last.node;
    return SThingListIterator(g_findResult ? g_findResult : last.node);
}
struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];
    CBaseIntelligentPointer PPot;
    long ActionPointIndex;
    bool HoldingBowl;
    long DayLastAte;
    CThing* GetPCookingPot();
};
CThing* CAIStateGroup_SitForMeal::GetPCookingPot()
{
    CThing* home = PBrain->PCreature->GetPHome();
    CTCBuildingContents* contents;
    if (home != 0 && home->GetInterface(&contents, 0x21)) {
        SThingListIterator first = contents->Things.begin();
        SThingListIterator it = FindIfCookingPot(first, contents->Things.end(), CIsCookingPot());
        if (it.node != contents->Things.end().node)
            return it.node->value.GetPItem();
    }
    return 0;
}
int main()
{
    static CThingAICreature creature; static CThing home, pot; static CTCBuildingContents contents; static CInterfaceMap::SEntry entries[2];
    static CAIBrainSitForMeal brain; static SThingListNode head, n1;
    memset(&home, 0, sizeof(home)); memset(&creature, 0, sizeof(creature));
    entries[0].type = 0x05; entries[1].type = 0x21; entries[1].component = &contents;
    home.m_interfaces.first = entries; home.m_interfaces.last = entries + 2; home.m_presentBits[1] |= 1UL << (0x21 & 31);
    contents.Things.head = &head; head.next = &n1; n1.next = &head; n1.value.m_pItem = &pot;
    brain.PCreature = &creature; creature.PHome.m_pItem = &home;
    CAIStateGroup_SitForMeal sg; sg.PBrain = &brain;
    g_findCalls = 0; g_findResult = &n1;
    CHECK(sg.GetPCookingPot() == &pot); CHECK(g_findCalls == 1); CHECK(g_findFirst == &n1 && g_findLast == &head);
    g_findResult = 0; g_findCalls = 0; CHECK(sg.GetPCookingPot() == 0); CHECK(g_findCalls == 1);
    home.m_presentBits[1] = 0; g_findCalls = 0; CHECK(sg.GetPCookingPot() == 0); CHECK(g_findCalls == 0);
    creature.PHome.m_pItem = 0; CHECK(sg.GetPCookingPot() == 0); CHECK(g_findCalls == 0);
    if (fails == 0) printf("00945a90_TEST PASS\n"); else printf("00945a90_TEST FAIL\n");
    return fails;
}