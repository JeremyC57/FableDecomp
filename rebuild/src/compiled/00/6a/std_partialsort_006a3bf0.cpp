// std::__partial_sort(SThingEntry* first, SThingEntry* middle, SThingEntry* last, SThingEntry*, CKeyPairCompareLess comp) -- STLport heap-based partial sort of (long, CIntelligentPointer<CThing>) pairs: heap [first, middle), pop in smaller tail elements, sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3BF0, length 123 bytes (+ int3 padding), `ret 0Ch`.
// Callees: std::__make_heap @ 0x006A3520, std::__pop_heap @ 0x006A34A0 (both this chunk), std::sort_heap @ 0x006A38C0
//   (landed), CBaseIntelligentPointer ctor @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90;
//   CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: the introsort fallback of the CVectorMap<long, CIntelligentPointer<CThing>> sort. The catalog's
//   "Heap_BuildHeap" is a descriptive auto label; the body is STLport's `__partial_sort(__first, __middle, __last,
//   _Tp*, __comp)`: make_heap (inline, forwarding null value/distance pointers), the pop loop over `__i < __last`
//   with `_Tp(*__i)` built in the argument area, then sort_heap.
// Shape: /Gr TU (__fastcall: first ecx, middle edx; last, the unused _Tp* and the comparator on the stack); the
//   comparator word is held in esi and reloaded from its slot after each __pop_heap call.
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

void __fastcall __make_heap(SThingEntry* __first, SThingEntry* __last, CKeyPairCompareLess __comp, SThingEntry*, int*);   // 0x006A3520
void __fastcall __pop_heap(SThingEntry* __first, SThingEntry* __last, SThingEntry* __result, SThingEntry __val,
                           CKeyPairCompareLess __comp, int*);   // 0x006A34A0
void __fastcall sort_heap(SThingEntry* __first, SThingEntry* __last, CKeyPairCompareLess __comp);   // 0x006A38C0

inline void make_heap(SThingEntry* __first, SThingEntry* __last, CKeyPairCompareLess __comp)
{
    __make_heap(__first, __last, __comp, (SThingEntry*)0, (int*)0);
}

void __fastcall __partial_sort(SThingEntry* __first, SThingEntry* __middle, SThingEntry* __last, SThingEntry*, CKeyPairCompareLess __comp)
{
    make_heap(__first, __middle, __comp);
    for (SThingEntry* __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, SThingEntry(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

}