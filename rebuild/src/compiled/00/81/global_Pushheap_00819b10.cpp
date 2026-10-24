// _global::_Push_heap(pair<EHeroMorphType,CTextureMorphEntry>*, long, long, pair<EHeroMorphType,CTextureMorphEntry>, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>) -- heap sift-up of hero texture-morph entries by morph type
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819B10, 145 bytes, `ret 0x20`.  No callees.
// Called by the landed _Adjust_heap @ 0x0081AA50 (same instantiation; its element model is reused here).
// Element (24 bytes): EHeroMorphType key +0, CTextureMorphEntry +4 (PDB, 20 bytes: MorphType,
// TextureLayer, ReplacingTextureIndex, BankIndex, Blend).  The key is a signed enum
// (`cmp [parent],ebx ; jge`, value.first loaded once into ebx); every move is the implicit 6-dword
// assignment through ebp, and the final drop stores the six value words.
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 24-byte pair by
// value and the empty comparator (4 + 0x18 + 4 = 0x20).  Byte twin of 0x0071F710.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace N00819b10 {
enum EHeroMorphType { HERO_MORPH_NONE = 0 };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };

struct CTextureMorphEntry {
    EHeroMorphType MorphType;                   // +0x00
    long TextureLayer;                          // +0x04
    long ReplacingTextureIndex;                 // +0x08
    long BankIndex;                             // +0x0c
    ECompositeBlendType Blend;                  // +0x10
    CTextureMorphEntry(const CTextureMorphEntry& other) { *this = other; }
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<EHeroMorphType, CTextureMorphEntry> TEntry;

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<EHeroMorphType, CTextureMorphEntry> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}