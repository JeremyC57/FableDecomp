// std::__make_heap(SThingEntry* first, SThingEntry* last, CKeyPairCompareLess comp, SThingEntry*, int*) -- STLport make_heap body: sift every parent of the (long, CIntelligentPointer<CThing>) range down, last parent first
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3520, length 124 bytes, `ret 0Ch`.
// Callees: std::__adjust_heap @ 0x006A3300 (landed as `_Adjust_heap`), CBaseIntelligentPointer ctor @ 0x00A01B10,
//   ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: called by __partial_sort 0x006A3BF0 as `make_heap(first, middle, comp)` (`push 0 ; push 0 ; push comp`:
//   the inline make_heap passes _STLP_VALUE_TYPE / _STLP_DISTANCE_TYPE null pointers). The catalog's
//   "Heap_Heapify" is a descriptive auto label.
// Element: pair<long, CIntelligentPointer<CThing> > (landed _Adjust_heap model); `_Tp(*(__first + __parent))` is
//   copy-constructed straight into the argument area (key, base ctor, vtable store, Get/SetPItem).
// Shape: /Gr TU (__fastcall: first ecx, last edx); `first` is homed in the `push ecx` frame word because ecx
//   carries the calls; len = (last - first) / 12 once (`cmp ebp,2 ; jl`), parent = (len - 2) / 2 (signed `cdq`
//   halving), the element pointer strength-reduced in ebx (`sub ebx,0Ch`).
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

void __fastcall __adjust_heap(SThingEntry* __first, int __holeIndex, int __len, SThingEntry __val, CKeyPairCompareLess __comp);   // 0x006A3300

void __fastcall __make_heap(SThingEntry* __first, SThingEntry* __last, CKeyPairCompareLess __comp, SThingEntry*, int*)
{
    if (__last - __first < 2) return;
    int __len = __last - __first;
    int __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, SThingEntry(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

}