// std::__unguarded_linear_insert(SThingEntry* last, SThingEntry val, CKeyPairCompareLess comp) -- STLport insertion step: shift greater-keyed (long, CIntelligentPointer<CThing>) pairs up one slot, drop val into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2DC0, length 87 bytes (+ int3 padding), `ret 10h`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90, ::~CBaseIntelligentPointer
//   @ 0x00A01C10.
// Identity: called by __linear_insert 0x006A3830 (ecx = last, the 12-byte value and the empty comparator on the
//   stack). The catalog's "Vector_MoveBackElements" is a descriptive auto label; the body is STLport's
//   `__unguarded_linear_insert(__last, __val, __comp)` over the CVectorMap<long, CIntelligentPointer<CThing>> sort.
// Element: pair<long, CIntelligentPointer<CThing> > (12 bytes; model of the landed _Adjust_heap 0x006A3300): the
//   implicit operator= is the key move then SetPItem(other.GetPItem()); the by-value pair dies through the base
//   destructor at the end. The key compare is the signed `jl`/`jge`.
// Shape: /Gr TU (__fastcall); `next = last - 1` is kept in esi, the shifted-from key stays in eax between the
//   compare and the key store (`mov [edi],eax`).
class CThing;
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

template <class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;   // 12 bytes: key +0, pointer +4 (vptr) / +8 (item)

struct CKeyPairCompareLess {                            // empty comparator, passed by value
    bool operator()(const SThingEntry& a, const SThingEntry& b) const { return a.first < b.first; }
};

namespace std {

void __fastcall __unguarded_linear_insert(SThingEntry* __last, SThingEntry __val, CKeyPairCompareLess __comp)
{
    SThingEntry* __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

}