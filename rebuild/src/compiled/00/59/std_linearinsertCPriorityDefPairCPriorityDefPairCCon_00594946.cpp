// std::__linear_insert<CPriorityDefPair*, CPriorityDefPair, CContextSensitivePriorityCompare>(first, last, val, comp) -- insertion-sort step: a new front element shifts the whole sorted run, otherwise an unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x00594946, 95 bytes, `ret 0Ch` (`/Gr` `__fastcall`: first in ecx, last in edx; the
// 8-byte by-value val and the 4-byte comparator slot on the stack).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes) with out-of-line copy ctor
// 0x00592DFB, assignment 0x0059308A and destructor 0x005930F3 (the callee destroys its by-value val); ordered by
// CContextSensitivePriorityCompare::operator() const @ 0x005928A8 (landed). Callees: STLport `__copy_backward(first,
// last, result, const random_access_iterator_tag&, ptrdiff_t*)` @ 0x005930C4 (`copy_backward` and its pointer
// dispatch inline), `__unguarded_linear_insert(last, val, comp)` @ 0x00593924 (this chunk).
// Only caller: the STLport `__insertion_sort` loop at 0x00594B7B.
// Label: the catalog "PriorityQueue_InsertSorted" is descriptive: the body is STLport 4.x `__linear_insert` verbatim:
// `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else __unguarded_linear_insert(last,
// val, comp);` -- the `random_access_iterator_tag` temporary homed in the high byte of the empty comparator's slot
// (`lea eax,[ebp+13h]`), the null distance pointer pushed as `0`.
// Shape: first in edi, last in esi. Frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

class CContextSensitiveItemDef;

template <class T> class CDefPointer {
public:
    T* PDef;                                         // the reference-counted def (released by ~pair 0x005930F3)
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& other);                         // 0x00592DFB
    pair& operator=(const pair& other);              // 0x0059308A
    ~pair();                                         // 0x005930F3
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

// Same-TU stand-in for the out-of-line `__copy_backward` (its STLport body): with it visible VC7.1 homes the empty
// tag in the comparator slot's high byte, as retail (declared only, the tag gets a frame word of its own).
#pragma auto_inline(off)
CPriorityDefPair* __fastcall __copy_backward(CPriorityDefPair* __first, CPriorityDefPair* __last,
                                             CPriorityDefPair* __result, const random_access_iterator_tag&,
                                             ptrdiff_t*)                                     // 0x005930C4
{
    for (ptrdiff_t __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)
void __fastcall __unguarded_linear_insert(CPriorityDefPair* __last, CPriorityDefPair __val,
                                          CContextSensitivePriorityCompare __comp);           // 0x00593924

template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert(CPriorityDefPair*, CPriorityDefPair*, CPriorityDefPair,
                                         CContextSensitivePriorityCompare);
}