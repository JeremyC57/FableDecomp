// Behaviour test for 0x007B8AA0 CTCTavernTable::FreeSeat(long seat, long point): the point index goes to the parent
// thing's interface-0x2E free-point component, then the seat is marked free (NumPeopleSeated drops only when the seat
// was used; out-of-range seats leave the vector and count alone).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __declspec(x)
#endif
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


static long g_lastPoint = -1;
static int g_calls = 0;
void CTCFreePointList::AddFreePoint(long index) { g_lastPoint = index; ++g_calls; }
void CTCTavernTable::v00() { }

int main()
{
    static CThing thing;
    memset(&thing, 0, sizeof(thing));
    thing.m_presentBits[0x2E >> 5] |= 1UL << (0x2E & 31);
    CInterfaceMap::SEntry entries[3];
    static CTCFreePointList freeList;
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x2E; entries[1].component = (CTCBase*)&freeList;
    entries[2].type = 0x40; entries[2].component = 0;
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + 3;

    bool seats[4] = { true, false, true, true };
    CTCTavernTable t;
    t.ParentThing = &thing;
    t.SeatsCurrentlyUsed._M_start = seats;
    t.SeatsCurrentlyUsed._M_finish = seats + 4;
    t.SeatsCurrentlyUsed._M_end_of_storage = seats + 4;
    t.NumPeopleSeated = 3;

    int ok = 1;
    t.FreeSeat(0, 7);                                   // used seat: freed, count drops
    if (g_lastPoint != 7 || g_calls != 1 || seats[0] != false || t.NumPeopleSeated != 2) ok = 0;
    t.FreeSeat(1, 9);                                   // already free: count unchanged
    if (g_lastPoint != 9 || g_calls != 2 || seats[1] != false || t.NumPeopleSeated != 2) ok = 0;
    t.FreeSeat(3, 11);                                  // last seat
    if (g_lastPoint != 11 || g_calls != 3 || seats[3] != false || t.NumPeopleSeated != 1) ok = 0;
    t.FreeSeat(4, 13);                                  // out of range: point still returned, nothing else
    if (g_lastPoint != 13 || g_calls != 4 || t.NumPeopleSeated != 1 || seats[2] != true) ok = 0;
    t.FreeSeat(-1, 15);                                 // negative seat is out of range (unsigned compare)
    if (g_lastPoint != 15 || g_calls != 5 || t.NumPeopleSeated != 1 || seats[2] != true) ok = 0;
    t.SetSeatState(1, true);                            // inline helper still counts up
    if (seats[1] != true || t.NumPeopleSeated != 2) ok = 0;

    printf(ok ? "007b8aa0_TEST PASS\n" : "007b8aa0_TEST FAIL\n");
    return ok ? 0 : 1;
}