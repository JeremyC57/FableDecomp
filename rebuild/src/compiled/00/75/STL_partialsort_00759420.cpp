// std::__partial_sort(pair<EBodyLocation,CWoundMorphs::CEntry>* first, pair<EBodyLocation,CWoundMorphs::CEntry>* middle, pair<EBodyLocation,CWoundMorphs::CEntry>* last, pair<EBodyLocation,CWoundMorphs::CEntry>*, CKeyPairCompareLess comp) -- heap-select the smallest entries into [first, middle) and sort them
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759420, 173 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, middle in edx,
//   then last, the value-type tag pointer and the empty comparator on the stack).
// Callees: __make_heap @ 0x00758C90 (first, middle, comp, 0, 0), __adjust_heap @ 0x00758910 (the inlined __pop_heap),
//   sort_heap @ 0x007590C0 (landed as `_Sort_heap`, out of line, `ret 4`).
// Callers: 0x0075953C and 0x00759755 (the CWoundMorphs map's introsort fallback and its partial_sort front end).
// Label: the manifest's `swap<CDebugSphere>` is a propagation; the body is STLport `__partial_sort` over the 0x2C-byte
//   wound-morph map entries.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap
//   and __pop_heap inline: the by-value T(*i) is a frame temporary (pair copy ctor: key dword, then `rep movsd` x10),
//   `*i = *first` the implicit `rep movsd` x11, the __adjust_heap value argument is copied memberwise into the argument
//   area; middle is homed in the frame and reloaded after each pop.
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

void __fastcall __make_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp, TWoundPair*, int*);           // 0x00758C90
void __fastcall __adjust_heap(TWoundPair* first, int holeIndex, int len, TWoundPair val, CKeyPairCompareLess comp);    // 0x00758910
void __fastcall sort_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp);                              // 0x007590C0

inline void make_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp)
{
    __make_heap(first, last, comp, (TWoundPair*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __fastcall __partial_sort(TWoundPair* first, TWoundPair* middle, TWoundPair* last, TWoundPair*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (TWoundPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, TWoundPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
}