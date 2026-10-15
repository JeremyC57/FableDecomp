// CAIStateGroup_SitForMeal::GetPCookingPot() -- find the cooking pot among the things of the creature's home
// Grade: DIFFER(127v121) -- frame only: the lane gives `sub esp,8` (lookup key in one word, predicate + returned iterator in the other) where retail overlays key, predicate and iterator in one `push ecx` slot; every instruction otherwise identical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945A90, length 121 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the home at CThingAICreature+0x168, then the found
// element), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x21 = bit 1 of the presence word
// at +0x24), std::find_if<list iterator, CIsCookingPot> @ 0x00945FD0 (/Gr algorithm TU: hidden result in ecx,
// both iterators and the 1-byte predicate on the stack, `ret 0Ch`).
//
// Shape: `home != 0 && home->GetInterface(&contents, 0x21)` (the bool lookup is the second `je` to the null
// tail), then find_if over the contents list with a VALUE-INITIALISED empty predicate (`CIsCookingPot()` with
// no user constructor gives the `mov byte ptr [esp+4],0` before the dword push); the key, the predicate and
// the returned iterator all share the single `push ecx` frame slot. A hit returns the element's GetPItem, a
// miss (end() re-read from the list head) or no home returns null.
struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// The building's contents component (interface 0x21) keeps a VC6-era Dinkumware std::list of
// CIntelligentPointer<CThing> at +0x08 ({allocator byte, head, size}: head at +0x0c, node {next, prev, value}).
struct SThingListNode {
    SThingListNode* next;
    SThingListNode* prev;
    CBaseIntelligentPointer value;              // +0x08
};

struct SThingListIterator {                     // 4-byte iterator class: returned through a hidden pointer
    SThingListNode* node;
    SThingListIterator() {}
    SThingListIterator(SThingListNode* n) : node(n) {}
};

struct CThingList {
    unsigned char allocator;                    // +0x00 (empty allocator, padded)
    SThingListNode* head;                       // +0x04
    long size;                                  // +0x08
    SThingListIterator begin() const { return SThingListIterator(head->next); }
    SThingListIterator end() const { return SThingListIterator(head); }
};

struct CTCBuildingContents {                    // interface type 0x21 component
    unsigned char _pad[0x8];
    CThingList Things;                          // +0x08
};

struct CThingAICreature : CThing {
    unsigned char _pad2[0x168 - sizeof(CThing)];
    CBaseIntelligentPointer PHome;              // +0x168 CIntelligentPointer<CThingBuilding>
    CThing* GetPHome() const { return PHome.GetPItem(); }   // inline accessor: `lea ecx,[eax+168h]`
};

struct CIsCookingPot {                          // empty predicate (no user constructor)
};

SThingListIterator __fastcall FindIfCookingPot(SThingListIterator first, SThingListIterator last,
                                               CIsCookingPot pred);   // 0x00945FD0

struct CAIBrainSitForMeal {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrainSitForMeal* PBrain;                 // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
};

struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];                 // +0x14
    CBaseIntelligentPointer PPot;               // +0x34 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
    CThing* GetPCookingPot();
};

CThing* CAIStateGroup_SitForMeal::GetPCookingPot()
{
    CThing* home = PBrain->PCreature->GetPHome();
    CTCBuildingContents* contents;
    if (home != 0 && home->GetInterface(&contents, 0x21)) {
        SThingListIterator it = FindIfCookingPot(contents->Things.begin(), contents->Things.end(), CIsCookingPot());
        if (it.node != contents->Things.end().node)
            return it.node->value.GetPItem();
    }
    return 0;
}
