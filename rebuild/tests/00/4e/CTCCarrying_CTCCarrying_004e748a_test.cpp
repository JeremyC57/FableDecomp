#include <cstdio>

class CThing { public: int id; };
class CTCBase { public:
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct SCarrySlot { long key; void* thing; };
struct CRetailVector_SCarrySlot {
    SCarrySlot* m_pFirst; SCarrySlot* m_pLast; SCarrySlot* m_pEnd;
    CRetailVector_SCarrySlot() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};
struct CKeyPairCompareLess { };
struct CVectorMap_CarrySlots {
    CRetailVector_SCarrySlot Vector; CKeyPairCompareLess Compare; bool Dirty;
    CVectorMap_CarrySlots() : Dirty(false) { }
};
struct CListNode { CListNode* next; CListNode* prev; };
static CListNode g_sentinel;
struct CRetailList_CIntelligentPointer {
    CListNode* head;
    CRetailList_CIntelligentPointer() : head(&g_sentinel) { g_sentinel.next = g_sentinel.prev = &g_sentinel; }
};
class CTCCarrying : public CTCBase { public:
    CVectorMap_CarrySlots           CarrySlots;
    bool                            BypassFilters;
    void*                           Key;
    CRetailList_CIntelligentPointer ThingsInPrimarySlots;
    CTCCarrying(CThing* owner) : CTCBase(owner), CarrySlots(), BypassFilters(false), Key(0), ThingsInPrimarySlots() { }
};

int main()
{
    CThing owner = { 1 };
    CTCCarrying c(&owner);
    if (c.ParentThing != &owner || c.CarrySlots.Vector.m_pFirst || c.CarrySlots.Vector.m_pLast || c.CarrySlots.Vector.m_pEnd ||
        c.CarrySlots.Dirty || c.BypassFilters || c.Key || c.ThingsInPrimarySlots.head->next != c.ThingsInPrimarySlots.head)
        { std::printf("004e748a_TEST FAIL\n"); return 1; }
    std::printf("004e748a_TEST PASS\n");
    return 0;
}