// std::__partial_sort(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* middle, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing>*, ConstThingCompare comp) -- STLport partial sort of the shop state group's thing pointers: heapify [first, middle), pop every tail element that sorts before the top into the heap, then sort the heap
// Grade: DIFFER(276v276) -- esi/edi swapped throughout (retail: first in esi, the copy temporaries and the reloaded comparator pointer in edi); tried __forceinline / inline __pop_heap and a non-forceinline copy constructor
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00927910, 276 bytes (+ int3 padding), /Gr `__fastcall` (first ecx, middle edx;
//   last, the _Tp* tag and the comparator pointer on the stack), `ret 0Ch`, an `and esp,-8` aligned ebp frame.
// Identity: the catalog's "Sort_MergeHeap" is descriptive. Callers: the same sort's __introsort_loop 0x00927AA0 (landed;
//   partial_sort(first, last, last, comp) inlined around the call at 0x00927A8C) and 0x00927B4B. The element is
//   CIntelligentPointer<const CThing> (8 bytes: vtable 0x01237F1C + the CBaseIntelligentPointer link), the comparator a
//   plain `bool (__fastcall*)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>)` (the
//   CAIStateGroup_OpenAndCloseShop TU, as the landed sort_heap 0x00927810 / __make_heap 0x009274B0 model it).
// Callees: __make_heap 0x009274B0, __adjust_heap 0x00926850, sort_heap 0x00927810 (landed), CBaseIntelligentPointer
//   ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10.
// Shape: STLport 4.6 `__partial_sort` verbatim (the landed CIntelligentPointer<CThing> twin 0x0080A240): both
//   comparator arguments are copy-constructed in the outgoing area (the `*first` copy first: right-to-left), the
//   comparator pointer is reloaded from its slot into edi for the call; __pop_heap is inlined: T(*i) in the aligned
//   frame temporary, `*i = *first`, the by-value copy for __adjust_heap, then the temporary destroyed.
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);                                                             // 0x009274B0
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                      // 0x00926850
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00927810

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (ConstThingPointer*)0, (int*)0);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                              _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(ConstThingPointer*, ConstThingPointer*, ConstThingPointer*, ConstThingPointer*,
                                        ConstThingCompare);
}
