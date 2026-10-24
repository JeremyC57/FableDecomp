// std::sort_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over (EHeroMorphType, CTextureMorphEntry) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081C670, length 139 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton. Stride 0x18 (the 0x2AAAAAAB / sar 2 divide by 24). The callee 0x0081B910 (manifest
// `_Pop_heap<pair<unsigned long,CSoundPair>*,...>`, propagated: PDB CSoundPair is 8 bytes) is the SGI
// `__pop_heap` of this 24-byte pair (`ret 24h`); it forwards to the manifest's `_Adjust_heap<pair<EHeroMorphType,
// CTextureMorphEntry>*,...>` @ 0x0081AA50, which sifts on a SIGNED key -- consistent with Ego_r.pdb
// CTextureMorphEntry (20 bytes: MorphType, TextureLayer, ReplacingTextureIndex, BankIndex, Blend).
// Callees: __pop_heap @ 0x0081B910 (out of line, __fastcall).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy is the pair's memberwise user copy constructor: the key dword, then the
// trivially copyable entry as a block through its own address (`add eax,4 ; lea edx,[esi+4]`, five dwords).
struct CTextureMorphEntry {                 // Ego_r.pdb, 20 bytes, trivially copyable
    long MorphType;                         // +0x00 EHeroMorphType
    long TextureLayer;                      // +0x04
    long ReplacingTextureIndex;             // +0x08
    long BankIndex;                         // +0x0c
    long Blend;                             // +0x10 ECompositeBlendType
};
struct STextureMorphPair {                  // std::pair<EHeroMorphType, CTextureMorphEntry> (0x18 bytes)
    long               first;               // +0x00 EHeroMorphType
    CTextureMorphEntry second;              // +0x04
    STextureMorphPair(const STextureMorphPair& other) : first(other.first), second(other.second) { }
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const STextureMorphPair& a, const STextureMorphPair& b) const { return a.first < b.first; }
};

void __fastcall __pop_heap(STextureMorphPair* first, STextureMorphPair* last, STextureMorphPair* result,
                           STextureMorphPair value, CKeyPairCompareLess comp, int*);   // 0x0081B910

inline void __pop_heap_aux(STextureMorphPair* first, STextureMorphPair* last, STextureMorphPair*,
                           CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, STextureMorphPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (STextureMorphPair*)0, comp);
}

void __fastcall sort_heap(STextureMorphPair* first, STextureMorphPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}