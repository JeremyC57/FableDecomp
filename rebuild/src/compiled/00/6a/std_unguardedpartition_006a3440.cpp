// std::__unguarded_partition(SThingEntry* first, SThingEntry* last, SThingEntry pivot, CKeyPairCompareLess comp) -- STLport quicksort partition of (long, CIntelligentPointer<CThing>) pairs around a by-value pivot
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3440, length 80 bytes, `ret 10h`.
// Callees: std::iter_swap(SThingEntry*, SThingEntry*) @ 0x006A2D40 (this chunk), CBaseIntelligentPointer::
//   ~CBaseIntelligentPointer @ 0x00A01C10 (the by-value pivot).
// Identity: the catalog's "Heap_SiftDownRecursive" is a descriptive auto label; the body is STLport's
//   `__unguarded_partition(__first, __last, __pivot, __comp)`: advance first past smaller keys, retreat last past
//   greater keys, stop when they cross, else iter_swap and step. Element and comparator as the landed
//   _Adjust_heap 0x006A3300 (signed key compare).
// Shape: /Gr TU (__fastcall: first in ecx, last in edx; the 12-byte pivot and the empty comparator on the stack);
//   the pivot key is re-read from its argument slot at the top of every outer pass; the pivot dies at the one exit.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {                        // 8 bytes: vptr, item block
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
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

void __fastcall iter_swap(SThingEntry* __a, SThingEntry* __b);   // 0x006A2D40

SThingEntry* __fastcall __unguarded_partition(SThingEntry* __first, SThingEntry* __last, SThingEntry __pivot, CKeyPairCompareLess __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

}