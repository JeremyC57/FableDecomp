// std::__unguarded_partition(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing> pivot, ConstThingCompare comp) -- STLport introsort partition step of the shop state group's thing-pointer sort: walk both ends towards the pivot and swap each out-of-place pair, returning the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00927590, 280 bytes (+ int3 padding), /Gr `__fastcall` (first ecx, last edx; the
//   8-byte pivot by value and the comparator pointer on the stack), `ret 0Ch`; the pivot is destroyed by the callee.
// Identity: the catalog's "Sort_Partition_Predicate" is descriptive. Caller: __introsort_loop 0x00927AA0 (landed, call
//   at 0x00927B16). Element and comparator as the landed sort_heap 0x00927810: CIntelligentPointer<const CThing>
//   (vtable 0x01237F1C) under `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, the
//   comparator through the pointer kept in ebp.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if
//   (!(first < last)) return first; iter_swap(first, last); ++first; }` with iter_swap -> __iter_swap inlined
//   (`T tmp = *a; *a = *b; *b = tmp;` in a frame temporary, destroyed after the swap); each comparator call takes two
//   by-value copies built in the outgoing area (the second argument first).
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    void* PData;                                // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C for const CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& p)
    {
        SetPItem(p.GetPItem());
        return *this;
    }
};
typedef CIntelligentPointer<const CThing> ConstThingPointer;
typedef bool (__fastcall* ConstThingCompare)(ConstThingPointer, ConstThingPointer);

namespace std {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, (ConstThingPointer*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
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

template ConstThingPointer* __fastcall __unguarded_partition(ConstThingPointer*, ConstThingPointer*, ConstThingPointer,
                                                             ConstThingCompare);
}