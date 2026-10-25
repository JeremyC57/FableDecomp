// Behaviour test for 0x009438F0 (source body copied from src/009438f0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
struct CTCBase;
// A thing's component table: the presence bitset at +0x20 (bit N of dword N/32) and the sorted {type, component}
// CVectorMap at +0x44 searched by LowerBound (non-const copy 0x0040F020, const copy 0x004365B0).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);         // 0x0040F020
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual unsigned long GetStateFlags();        // slot 48 (+0xC0): a word of 1-bit kind flags
    unsigned char _pad04[0xc];
    unsigned char m_thingType;                    // +0x10 (5: object)
    unsigned char _pad11[0xf];
    unsigned long m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                   // +0x44
    unsigned char _pad50[0x41];
    unsigned char m_flags91;                      // +0x91
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool GetInterfaceConst(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                     // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
struct CTCUserActionPointBase {
    bool IsActionPointFree();                     // 0x007FEBF0
    long GetNumberOfActionPointsInUse();          // 0x007FEC10
};

namespace _STL {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);               // 0x0074FAE0 for vector<CThing*> (out of line)
};
#pragma auto_inline(off)
// Same-TU stand-in for vector<CThing*>::push_back 0x0074FAE0 (reads the element, never keeps its address): with it VC7.1
// treats `seat` as uncaptured, keeps it in esi with its home in the push-ecx slot and puts the LowerBound key in the dead
// `thing` slot, as retail.
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage)
        *_M_finish++ = __x;
}
#pragma auto_inline(on)
}

class CAddOccupiedSeatToList {
public:
    _STL::vector<CThing*>* m_pSeats;                       // +0x00
    void operator()(const CIntelligentPointer<CThing>& thing) const;
};
void CAddOccupiedSeatToList::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (!thing.GetPItem() || thing.GetPItem()->m_thingType != 5)
        return;
    CThing* seat = thing.GetPItem();
    if (((unsigned char)(seat->GetStateFlags() >> 16) & 1) || ((unsigned char)(seat->GetStateFlags() >> 17) & 1)) {
        CTCUserActionPointBase* actionPoint;
        if (seat->GetInterfaceConst(0x2d, &actionPoint) && !actionPoint->IsActionPointFree())
            m_pSeats->push_back(seat);
    }
}

// ---- test harness ----
void CThing::v000() { }
void CThing::v001() { }
void CThing::v002() { }
void CThing::v003() { }
void CThing::v004() { }
void CThing::v005() { }
void CThing::v006() { }
void CThing::v007() { }
void CThing::v008() { }
void CThing::v009() { }
void CThing::v010() { }
void CThing::v011() { }
void CThing::v012() { }
void CThing::v013() { }
void CThing::v014() { }
void CThing::v015() { }
void CThing::v016() { }
void CThing::v017() { }
void CThing::v018() { }
void CThing::v019() { }
void CThing::v020() { }
void CThing::v021() { }
void CThing::v022() { }
void CThing::v023() { }
void CThing::v024() { }
void CThing::v025() { }
void CThing::v026() { }
void CThing::v027() { }
void CThing::v028() { }
void CThing::v029() { }
void CThing::v030() { }
void CThing::v031() { }
void CThing::v032() { }
void CThing::v033() { }
void CThing::v034() { }
void CThing::v035() { }
void CThing::v036() { }
void CThing::v037() { }
void CThing::v038() { }
void CThing::v039() { }
void CThing::v040() { }
void CThing::v041() { }
void CThing::v042() { }
void CThing::v043() { }
void CThing::v044() { }
void CThing::v045() { }
void CThing::v046() { }
void CThing::v047() { }
struct STestThing : CThing { unsigned long Flags; };
unsigned long CThing::GetStateFlags() { return ((STestThing*)this)->Flags; }
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
struct STestActionPoint { bool Free; };
bool CTCUserActionPointBase::IsActionPointFree() { return ((STestActionPoint*)this)->Free; }
long CTCUserActionPointBase::GetNumberOfActionPointsInUse() { return 0; }
static STestThing* MakeThing(unsigned char type, unsigned long flags, bool hasAP, STestActionPoint* ap, CInterfaceMap::SEntry* e)
{
    STestThing* t = new STestThing;
    memset((char*)t + sizeof(void*), 0, sizeof(STestThing) - sizeof(void*));
    t->m_thingType = type; t->Flags = flags;
    e[0].type = 0x10; e[0].component = 0;
    e[1].type = 0x2d; e[1].component = (CTCBase*)ap;
    t->m_interfaces.first = e; t->m_interfaces.last = e + 2; t->m_interfaces.end = e + 2;
    if (hasAP) t->m_presentBits[0x2d >> 5] |= (1UL << (0x2d & 31));
    return t;
}
static CIntelligentPointer<CThing> Ptr(CThing* t) { CIntelligentPointer<CThing> p; p.m_pItem = t; return p; }
int main()
{
    int ok = 1;
    static CThing* store[8];
    _STL::vector<CThing*> seats; seats._M_start = seats._M_finish = store; seats._M_end_of_storage = store + 8;
    CAddOccupiedSeatToList f; f.m_pSeats = &seats;
    STestActionPoint apFree; apFree.Free = true;
    STestActionPoint apBusy; apBusy.Free = false;
    CInterfaceMap::SEntry e1[2], e2[2], e3[2], e4[2], e5[2], e6[2];
    STestThing* freeSeat16 = MakeThing(5, 1UL << 16, true, &apFree, e1);
    STestThing* busySeat17 = MakeThing(5, 1UL << 17, true, &apBusy, e2);
    STestThing* notSeat = MakeThing(5, 1UL << 3, true, &apFree, e3);
    STestThing* notObject = MakeThing(4, 1UL << 16, true, &apFree, e4);
    STestThing* noAP = MakeThing(5, 1UL << 16, false, &apFree, e5);
    STestThing* freeSeat17 = MakeThing(5, 1UL << 17, true, &apFree, e6);
    f(Ptr(0));
    f(Ptr(freeSeat16)); f(Ptr(busySeat17)); f(Ptr(notSeat)); f(Ptr(notObject)); f(Ptr(noAP)); f(Ptr(freeSeat17));
    long n = (long)(seats._M_finish - seats._M_start);
    if (0) {
        if (n != 2 || store[0] != freeSeat16 || store[1] != freeSeat17) ok = 0;
    } else {
        if (n != 1 || store[0] != busySeat17) ok = 0;
    }
    printf("009438f0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}