// std::__make_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp, STextureMorphPair*, int*) -- heapify a vector of (EHeroMorphType, CTextureMorphEntry) pairs: sift every parent down from the last one to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081BAD0, 123 bytes (+int3 pad), `ret 0Ch` (link-time __fastcall /Gr TU: first in
//   ecx, last in edx, then the empty comparator and the value-type / distance-type tag pointers on the stack).
// Callee: __adjust_heap(first, parent, len, T(*(first + parent)), comp) @ 0x0081AA50 (landed; manifest
//   `_Adjust_heap<pair<EHeroMorphType,CTextureMorphEntry>*,...>`, `ret 20h`).
// Callers: 0x0081BEE9 (make_heap front end) and 0x0081D461 (the 24-byte __partial_sort 0x0081D450).
// Label: the catalog's "std_make_heap_24ByteElement" is descriptive; the body is STLport `__make_heap`.
// Identity: the 0x2AAAAAAB / sar 2 divide is the 24-byte stride of the landed sort_heap 0x0081C670 / __pop_heap 0x0081B910
//   family: Ego_r.pdb CTextureMorphEntry (20 bytes: MorphType, TextureLayer, ReplacingTextureIndex, BankIndex, Blend)
//   behind a 4-byte morph-type key.
// Shape: the landed 0x00758C90 form: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;)
//   { __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` with the
//   cursor first + parent strength-reduced into ebx; the by-value pair is built in the argument area by STLport pair's
//   memberwise user copy constructor: the key dword, then the trivially copyable entry through its own address
//   (`add eax,4 ; lea ecx,[ebx+4]`, five dwords) -- the 0x0081C670 model.
struct CTextureMorphEntry {                 // Ego_r.pdb, 20 bytes, trivially copyable
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

void __fastcall __adjust_heap(STextureMorphPair* first, int holeIndex, int len, STextureMorphPair val, CKeyPairCompareLess comp);   // 0x0081AA50

void __fastcall __make_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp, STextureMorphPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, STextureMorphPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}
}