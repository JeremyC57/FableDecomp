// CTCTavernTable::FreeSeat(long seat, long point) -- hand the seat's point index back to the parent thing's interface-0x2E free-point component, then mark the seat free (counting NumPeopleSeated down when it was used)
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined SetSeatState indexes the seat element twice instead of through a bool& local (the 0x007B8530 recipe), which keeps the vector start in esi across the size check as retail does
// Leaf: ?FreeSeat@CTCTavernTable@@QAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8AA0, 111 bytes (+1 int3), thiscall `ret 8`.
// Identity: Ego_r.pdb ?FreeSeat@CTCTavernTable@@QAEXJJ@Z (long seat, long point). The body is the landed
//   CTCTavernTable::FreePoint (0x007B9090) followed by the inline seat-free of SetSeatState (0x007B8530) with
//   used = false. The catalog label "CSeatingArea_FreeSeat" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inline GetInterface, type 0x2E = bit 14
//   of the bitset dword at +0x24, `test ch,40h`), the free-point component's AddFreePoint @ 0x007FEF40.
// Layout: CTCBase ParentThing +0x04; CTCTavernTable SeatsCurrentlyUsed +0x34 (STLport vector, one byte per seat),
//   NumPeopleSeated +0x40.
// Shape: the component pointer is an uninitialised local filled by the unchecked inline GetInterface; on the miss
//   path retail reads the never-written local, which VC7.1 homed in the `point` argument slot (`mov eax,[esp+14h]`).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

struct CTCFreePointList {                       // interface type 0x2E component
    void AddFreePoint(long index);              // 0x007FEF40
};

template <class T> class CRetailVector {        // retail STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};

class CTCTavernTable {
public:
    virtual void v00();
    CThing*                  ParentThing;               // +0x04 (CTCBase)
    unsigned char            _pad08[4];                 // +0x08
    bool                     BeingServed;               // +0x0c
    CRetailVector<void*>     Occupants;                 // +0x10
    CRetailVector<void*>     Tankards;                  // +0x1c
    CRetailVector<void*>     Food;                      // +0x28
    CRetailVector<bool>      SeatsCurrentlyUsed;        // +0x34
    long                     NumPeopleSeated;           // +0x40
    void SetSeatState(long seat, bool used);
    void FreeSeat(long seat, long point);
};

void CTCTavernTable::SetSeatState(long seat, bool used)   // 0x007B8530, auto-inlined below (/O2 = /Ob2)
{
    if (SeatsCurrentlyUsed.size() > (unsigned long)seat) {
        if (SeatsCurrentlyUsed[seat] != used) {
            if (used)
                ++NumPeopleSeated;
            else
                --NumPeopleSeated;
        }
        SeatsCurrentlyUsed[seat] = used;
    }
}

void CTCTavernTable::FreeSeat(long seat, long point)
{
    CTCFreePointList* points;
    ParentThing->GetInterface(&points, 0x2E);
    points->AddFreePoint(point);
    SetSeatState(seat, false);
}