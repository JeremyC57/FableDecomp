// std::__unguarded_partition(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<EBodyLocation,CWoundMorphs::CEntry>* last, pair<EBodyLocation,CWoundMorphs::CEntry> pivot, CKeyPairCompareLess comp) -- quicksort partition of the wound-morph entries around a by-value pivot; returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758B70, 109 bytes (+3 int3), `ret 30h` (link-time __fastcall: first in ecx, last
//   in edx, the 0x2C-byte pivot and the empty comparator on the stack: 0x2C + 4 = 0x30). No callees.
// Caller: 0x00759708, the CWoundMorphs map's __introsort_loop (0x00759660, which the landed sort 0x007597C0 calls).
// Label: the manifest's "Sort_HeapsortPartition" is descriptive; the body is STLport `__unguarded_partition`.
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` with the signed key compare against the
//   pivot key held in ebx; the inlined `__iter_swap` temporary (pair copy ctor: key dword then `rep movsd` x10) is
//   homed in the dead pivot argument slot (`mov [esp+10h],ecx ; lea edi,[esp+14h]`), the two assignments are the
//   implicit `rep movsd` x11.
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

struct CWoundMorphsEntry {                       // CWoundMorphs::CEntry, 40 bytes of POD
    long Words[10];
};

namespace std {
template <class A, class B> struct pair {
    A first;                                     // +0x00
    B second;                                    // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<EBodyLocation, CWoundMorphsEntry> TWoundPair;

struct CKeyPairCompareLess {                     // empty, passed by value
    bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; }
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
    __iter_swap(a, b, (TWoundPair*)0);
}

TWoundPair* __fastcall __unguarded_partition(TWoundPair* first, TWoundPair* last, TWoundPair pivot, CKeyPairCompareLess comp)
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