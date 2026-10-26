// CAIStateGroup_SitForMeal::GetPFreeChair() -- find a free chair among the things of the creature's home
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945B10, length 122 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the home at CThingAICreature+0x168, then the found
// element), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x21 = bit 1 of the presence word
// at +0x24), std::find_if<list iterator, CIsFreeChairInBuilding> @ 0x00946020 (/Gr algorithm TU: hidden result
// in ecx, both iterators and the 4-byte predicate on the stack, `ret 0Ch`).
//
// Shape: the home is kept in edi (named local): it is null-tested, searched for the contents component (bool
// GetInterface), and pushed as the predicate's only member (a 4-byte predicate class constructed from it is
// pushed straight from the register). A found element is returned only when its thing flag word at +0x6c has
// bit 11 set (`test ch,8`), else null; the key and the returned iterator share the `push ecx` frame slot.
class CTCBase;

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
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
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
#pragma auto_inline(on)

class CThing { public:
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

// The building's contents component (interface 0x21) keeps a VC6-era Dinkumware std::list of
// CIntelligentPointer<CThing> at +0x08 ({allocator byte, head, size}: head at +0x0c, node {next, prev, value}).
struct SThingListNode {
    SThingListNode* next;
    SThingListNode* prev;
    CIntelligentPointer<CThing> value;              // +0x08
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

class CThingAICreature : public CThing { public:
    unsigned char _pad2[0x168 - sizeof(CThing)];
    CIntelligentPointer<CThing> PHome;              // +0x168 CIntelligentPointer<CThingBuilding>
    CThing* GetPHome() const { return PHome.GetPItem(); }   // inline accessor: `lea ecx,[eax+168h]`
};

struct CIsFreeChairInBuilding {                 // predicate holding the building being searched
    CThing* PBuilding;
    CIsFreeChairInBuilding(CThing* building) : PBuilding(building) {}
};

SThingListIterator __fastcall FindIfFreeChair(SThingListIterator first, SThingListIterator last,
                                              CIsFreeChairInBuilding pred);   // 0x00946020

struct CThingChair {                            // the found thing viewed through its flag word
    unsigned char _pad[0x6c];
    unsigned long ThingFlags;                   // +0x6c (bit 11: usable seat)
};

struct CAIBrainSitForMeal {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};

class CAIStateGroupBase { public:
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
    CIntelligentPointer<CThing> PPot;               // +0x34 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
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