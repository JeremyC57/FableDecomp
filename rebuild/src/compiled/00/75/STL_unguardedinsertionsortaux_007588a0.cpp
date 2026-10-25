// std::__unguarded_insertion_sort_aux(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<EBodyLocation,CWoundMorphs::CEntry>* last, pair<EBodyLocation,CWoundMorphs::CEntry>*, CKeyPairCompareLess comp) -- insertion-sort pass with no lower-bound test, each entry sifted down by its signed body-location key
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x007588A0, 96 bytes (+16 int3), `ret 8` (link-time __fastcall: first in ecx, last in
//   edx, the value-type tag pointer and the empty comparator on the stack). No callees.
// Callers: 0x00758BE7 and 0x007593F6 (inside the CWoundMorphs map sort family 0x00758xxx-0x007597C0: __final_insertion_sort
//   0x007593C0 runs it over [first + 16, last)).
// Label: the manifest's `_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>` (second row of that name) is a propagation; the
//   body is STLport's unguarded insertion sort over the 0x2C-byte map entries.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the linear insert
//   inlined: the by-value T(*i) is built in the frame by the pair's memberwise user copy constructor (key dword, then
//   the 40-byte POD second as `rep movsd` x10); `*last = *next` and `*last = val` are the implicit assignment
//   (`rep movsd` x11); the key compare is signed (`jge`/`jl`), the key kept in ebp.
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

template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall __unguarded_insertion_sort_aux(TWoundPair* first, TWoundPair* last, TWoundPair*, CKeyPairCompareLess comp)
{
    for (TWoundPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, TWoundPair(*i), comp);
}
}