// std::__partial_sort(STextureMorphPair* first, STextureMorphPair* middle, STextureMorphPair* last, STextureMorphPair*, CKeyPairCompareLess comp) -- heap-select the smallest (EHeroMorphType, CTextureMorphEntry) pairs into [first, middle) and sort them
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081D450, 120 bytes (+int3 pad), `ret 0Ch` (link-time __fastcall /Gr TU: first in ecx,
//   middle in edx, then last, the value-type tag pointer and the empty comparator on the stack).
// Callees: __make_heap @ 0x0081BAD0 (first, middle, comp, 0, 0), __pop_heap @ 0x0081B910 (landed, OUT OF LINE in this
//   instance: `ret 24h`, result/value/comp/distance on the stack), sort_heap @ 0x0081C670 (landed, `ret 4`).
// Label: the catalog's "std_make_partial_heap_6Element" is descriptive; the body is STLport `__partial_sort` over the
//   24-byte pairs of the landed sort_heap 0x0081C670 family.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first,
//   middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap inline and __pop_heap
//   called: the by-value T(*i) is built straight in the argument area by pair's memberwise copy ctor (key dword, then the
//   trivially copyable entry through its own address, `add eax,4 ; lea ecx,[esi+4]`); the comparator lives in ebp.
class CTextureMorphEntry { public:                 // Ego_r.pdb, 20 bytes, trivially copyable
    long MorphType;                         // +0x00 EHeroMorphType
    long TextureLayer;                      // +0x04
    long ReplacingTextureIndex;             // +0x08
    long BankIndex;                         // +0x0c
    long Blend;                             // +0x10 ECompositeBlendType
};

namespace std {
template <class A, class B> struct pair {
    A first;                                // +0x00
    B second;                               // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};

typedef pair<long, CTextureMorphEntry> STextureMorphPair;

struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const STextureMorphPair& a, const STextureMorphPair& b) const { return a.first < b.first; }
};

void __fastcall __make_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp, STextureMorphPair*, int*);   // 0x0081BAD0
void __fastcall __pop_heap(STextureMorphPair* first, STextureMorphPair* last, STextureMorphPair* result, STextureMorphPair val,
                           CKeyPairCompareLess comp, int*);                                                                         // 0x0081B910
void __fastcall sort_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp);                              // 0x0081C670

inline void make_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp)
{
    __make_heap(first, last, comp, (STextureMorphPair*)0, (int*)0);
}

void __fastcall __partial_sort(STextureMorphPair* first, STextureMorphPair* middle, STextureMorphPair* last, STextureMorphPair*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (STextureMorphPair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, STextureMorphPair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
}