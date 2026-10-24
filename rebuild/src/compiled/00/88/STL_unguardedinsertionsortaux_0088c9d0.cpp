// _STL::__unguarded_insertion_sort_aux(SKeyedPair* first, SKeyedPair* last, SKeyedPair*, SKeyGreater comp) -- unguarded insertion sort of the 8-byte keyed pairs of [first, last) (the tail pass of the final insertion sort)
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088C9D0, length 73 bytes (+ int3 padding), `ret 8`.
// Identity: callers 0x0088CAF7, 0x0088CFAC and the landed sort 0x0088D130 (0x0088D189), which declares this body as its
//   `__unguarded_insertion_sort_aux(first + 16, last, (T*)0, comp)`. The catalog label `_Copy_impl<CNavigationPosition*>`
//   is propagated. Element and comparator as the landed __push_heap 0x0088C7C0 (descriptive names).
// Shape: STLport 4.x `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` in a /Gr TU (`__fastcall`: first in
//   ecx, last in edx, the null value-type pointer and the empty comparator on the stack, `ret 8`) with the
//   __unguarded_linear_insert of `_Tp(*i)` inlined: the copied pair in edi/ebx, the shift loop through ebp.
struct SKeyedPair {                             // 8 bytes: a signed key and its payload (descriptive name)
    long          m_key;                        // +0x00
    unsigned long m_value;                      // +0x04
    SKeyedPair(const SKeyedPair& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct SKeyGreater {                            // empty comparator passed by value (one stack slot)
    char m_tag;
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.m_key > b.m_key; }
};

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<SKeyedPair*, SKeyedPair, SKeyGreater>(SKeyedPair*, SKeyedPair*, SKeyedPair*, SKeyGreater);
}