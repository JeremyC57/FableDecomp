// CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding(CThing* building) -- collect the free seats of the
// Grade: RELOCATION_MATCH -- landing retry: out-parameter GetInterface in the if (component lands in eax); byte twin of 0x00938BF0
// building's interface-0x21 component into a vector and return one picked with the world's random seed (or null).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009431E0, length 220 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + entry-returning FindEntry, 0x21 = bit 1 of +0x24),
// std::for_each<list iterator, CAddFreeSeat> @ 0x00938E90 (__fastcall, functor returned by hidden pointer; the
// manifest's "_Find_if<...CMiniMapDisplay...>" label is propagated), CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0,
// GFROR13 @ 0x00497890 (__fastcall), operator delete @ 0x00BFEA14 (the vector's storage, two paths).
// Shape: the lookup key and the returned functor both live in the dead `building` argument slot; the inline
// random step `seed = ROR13(seed * 0x24a1 + 0x24df)` and `count ? r % count : 0`.
// No engine header for this class; the seat std::list is embedded at +0x8 of the building component (head +0xc).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};


extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
long __fastcall GFROR13(unsigned long);   // 0x00497890

struct CThingVector {                           // std::vector<CThing*> (first, last, end of storage)
    CThing** m_first;
    CThing** m_last;
    CThing** m_end;
    CThingVector() : m_first(0), m_last(0), m_end(0) { }
    ~CThingVector() { if (m_first) free(m_first); }
    long size() const { return m_last - m_first; }
};

struct CSeatListNode { CSeatListNode* next; CSeatListNode* prev; };
struct CSeatListIterator {
    CSeatListNode* node;
    CSeatListIterator(CSeatListNode* n) : node(n) { }
};
struct CSeatList {                              // std::list of seats (VC7.1: allocator word, head node, size)
    unsigned char _alloc[4];
    CSeatListNode* head;
    unsigned long size;
    CSeatListIterator begin() const { return CSeatListIterator(head->next); }
    CSeatListIterator end() const { return CSeatListIterator(head); }
};

struct CAddFreeSeat {                           // functor: appends free seats to the vector
    CThingVector* m_pSeats;
    CAddFreeSeat(CThingVector* seats) : m_pSeats(seats) { }
};
CAddFreeSeat __fastcall ForEachSeat_00943990(CSeatListIterator first, CSeatListIterator last, CAddFreeSeat f);   // 0x00938E90

struct CTCBuildingSeats {                       // interface 0x21
    unsigned char _pad[0x8];
    CSeatList m_seats;                          // +0x08 (head node at +0x0c)
};

struct CAIStateGroupBaseR : CAIStateGroupBase {
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long GetRandom(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBaseR {
    CThing* GetRandomFreeSeatInBuilding(CThing* building);
};

CThing* CAIStateGroup_UsesSeat::GetRandomFreeSeatInBuilding(CThing* building)
{
    CTCBuildingSeats* comp;
    if (building->GetInterface(&comp, 0x21)) {
        CSeatListNode* head = comp->m_seats.head;
        CThingVector seats;
        ForEachSeat_00943990(CSeatListIterator(head->next), CSeatListIterator(head), CAddFreeSeat(&seats));
        if (seats.m_first != seats.m_last) {
            long count = seats.size();
            CThing* seat = seats.m_first[GetRandom(count)];
            return seat;
        }
    }
    return 0;
}