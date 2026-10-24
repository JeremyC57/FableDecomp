// std::__insertion_sort(SThingEntry* first, SThingEntry* last, CKeyPairCompareLess comp) -- STLport insertion sort of (long, CIntelligentPointer<CThing>) pairs: linear-insert every element after the first
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3B90, length 88 bytes (+ int3 padding), `ret 4`.
// Callees: std::__linear_insert @ 0x006A3830 (this chunk), CBaseIntelligentPointer ctor @ 0x00A01B10, ::GetPItem
//   @ 0x00A01B50, ::SetPItem @ 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: called by the landed __final_insertion_sort 0x006A3D30 (whose header names 0x006A3B90
//   `__insertion_sort`). The catalog's "Heap_BuildHeapFromRange" is a descriptive auto label; the body is STLport's
//   `if (__first == __last) return; for (__i = __first + 1; __i != __last; ++__i) __linear_insert(__first, __i, *__i, __comp);`
//   (the value supplied by the caller, copied into the argument area by the pair's implicit copy constructor).
// Shape: /Gr TU (__fastcall: first ecx, last edx, the empty comparator on the stack and re-pushed per call).
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

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
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

void __fastcall __linear_insert(SThingEntry* __first, SThingEntry* __last, SThingEntry __val, CKeyPairCompareLess __comp);   // 0x006A3830

void __fastcall __insertion_sort(SThingEntry* __first, SThingEntry* __last, CKeyPairCompareLess __comp)
{
    if (__first == __last) return;
    for (SThingEntry* __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

}