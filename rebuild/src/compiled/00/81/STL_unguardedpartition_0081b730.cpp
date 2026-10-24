// _STL::__unguarded_partition(STextureMorphPair* first, STextureMorphPair* last, STextureMorphPair pivot, CKeyPairCompareLess comp) -- quicksort partition of the (EHeroMorphType, CTextureMorphEntry) pairs around a by-value pivot; returns the cut
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081B730, 179 bytes, `ret 1Ch` (link-time __fastcall /Gr TU: first in ecx, last in
//   edx, the 0x18-byte pivot and the empty comparator on the stack: 0x18 + 4 = 0x1C). No callees.
// Label: the catalog's "std_partition_6ElementSwap" is descriptive; the body is STLport `__unguarded_partition` over the
//   24-byte pairs of the landed sort_heap 0x0081C670 / __adjust_heap 0x0081AA50 family (signed key compare).
// Shape: STLport `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last; if (!(first
//   < last)) return first; iter_swap(first, last); ++first; }`; the pivot key is re-read from its slot each round. The
//   inlined `__iter_swap` temporary lives in the 0x18-byte frame (key in ecx, the first payload word in esi): STLport
//   pair's memberwise user copy constructor copies the trivially copyable entry through its own address (`lea esi,[eax+4]
//   ; mov edi,esi`), the two assignments are the implicit six-dword copies through ebp.
struct CTextureMorphEntry {                 // Ego_r.pdb, 20 bytes, trivially copyable
    long MorphType;                         // +0x00 EHeroMorphType
    long TextureLayer;                      // +0x04
    long ReplacingTextureIndex;             // +0x08
    long BankIndex;                         // +0x0c
    long Blend;                             // +0x10 ECompositeBlendType
};

namespace _STL {
template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<long, CTextureMorphEntry> STextureMorphPair;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const STextureMorphPair& a, const STextureMorphPair& b) const { return a.first < b.first; }
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
    __iter_swap(a, b, (STextureMorphPair*)0);
}

STextureMorphPair* __fastcall __unguarded_partition(STextureMorphPair* first, STextureMorphPair* last, STextureMorphPair pivot, CKeyPairCompareLess comp)
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