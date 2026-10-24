// std::__partial_sort<CPriorityDefPair*, CPriorityDefPair, CContextSensitivePriorityCompare>(first, middle, last, T*, comp) -- heap-select the best [first, middle) context-sensitive items out of [first, last), then heap-sort them
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594C77, 91 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, middle in edx;
// last, the unused value-type pointer and the 4-byte comparator slot on the stack).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes), its out-of-line copy ctor
// pair(const pair&) @ 0x00592DFB building each by-value `_Tp(*i)` in its argument slot; ordered by
// CContextSensitivePriorityCompare::operator() const @ 0x005928A8 (landed). Callees: make_heap(first, last, comp)
// @ 0x005949A5 (forwards to `__make_heap` 0x00594706, this chunk), `__pop_heap(first, last, result, value, comp,
// ptrdiff_t*)` @ 0x005946B2, sort_heap(first, last, comp) @ 0x00594B8D.
// Only caller: partial_sort(first, middle, last, comp) @ 0x00594DE2, the depth-limit fallback of the STLport
// `__introsort_loop` @ 0x00594E4D (this chunk).
// Label: the catalog "Heap_Sort_CContextSensitive" is descriptive: the body is STLport 4.x `__partial_sort`
// verbatim: `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
// middle, i, _Tp(*i), comp, _STLP_DISTANCE_TYPE(first, _RandomAccessIter)); sort_heap(first, middle, comp);`.
// Shape: first in ebx, middle in esi, i in edi (a rotated `i < last` loop); the comparator slot is pushed by value to
// each callee. Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

class CContextSensitiveItemDef;

template <class T> class CDefPointer {
public:
    T* PDef;                                         // the reference-counted def
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& other);                         // 0x00592DFB
    ~pair();                                         // 0x005930F3
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
void __fastcall make_heap(CPriorityDefPair* __first, CPriorityDefPair* __last,
                          CContextSensitivePriorityCompare __comp);                          // 0x005949A5
void __fastcall sort_heap(CPriorityDefPair* __first, CPriorityDefPair* __last,
                          CContextSensitivePriorityCompare __comp);                          // 0x00594B8D
void __fastcall __pop_heap(CPriorityDefPair* __first, CPriorityDefPair* __last, CPriorityDefPair* __result,
                           CPriorityDefPair __value, CContextSensitivePriorityCompare __comp,
                           ptrdiff_t*);                                                      // 0x005946B2

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (ptrdiff_t*)0);
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort(CPriorityDefPair*, CPriorityDefPair*, CPriorityDefPair*, CPriorityDefPair*,
                                        CContextSensitivePriorityCompare);
}