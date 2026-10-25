// std::__unguarded_partition(SKeyedBlock40* first, SKeyedBlock40* last, SKeyedBlock40 pivot, CKeyPairCompareLess comp) -- quicksort partition of the (long key, 36-byte payload) pairs around a by-value pivot; returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081B800, 109 bytes, `ret 2Ch` (link-time __fastcall /Gr TU: first in ecx, last in
//   edx, the 0x28-byte pivot and the empty comparator on the stack: 0x28 + 4 = 0x2C). No callees.
// Label: the catalog's "std_partition_10ElementSwap" is descriptive; the body is STLport `__unguarded_partition`, the
//   40-byte instance of the landed 0x00758B70 (44-byte wound-morph pairs) -- the element is the (long, 36-byte POD) pair
//   of the landed _Sort_heap 0x0081C700 / sort 0x0081E090 family (signed key compare, `jge`/`jl`).
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if (!(first
//   < last)) return first; iter_swap(first, last); ++first; }` with the pivot key held in ebx; the inlined `__iter_swap`
//   temporary (pair copy ctor: key dword, then `rep movsd` x9) is homed in the dead pivot argument slot, the two
//   assignments are the implicit `rep movsd` x10.
struct SPayload36 {                         // 36-byte POD payload (copied as one block)
    unsigned long m_words[9];
};

namespace std {
template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<long, SPayload36> SKeyedBlock40;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SKeyedBlock40& a, const SKeyedBlock40& b) const { return a.first < b.first; }
};

template <class ForwardIter1, class ForwardIter2, class T>
inline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
inline void iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap(a, b, (SKeyedBlock40*)0);
}

SKeyedBlock40* __fastcall __unguarded_partition(SKeyedBlock40* first, SKeyedBlock40* last, SKeyedBlock40 pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}
}