// _global::_Adjust_heap(pair<EHeroMorphType,CTextureMorphEntry>*, long, long, pair<EHeroMorphType,CTextureMorphEntry>, CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>) -- heap sift-down of hero texture-morph entries by morph type, then _Push_heap @ 0x00819B10
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081AA50, 208 bytes.
// Callee: 0x00819B10 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in
// edx, then bottom, the 24-byte pair by value and the 4-byte comparator; `ret 0x20`.
// Element (24 bytes): EHeroMorphType key +0, CTextureMorphEntry +4 (PDB, 20 bytes: MorphType,
// TextureLayer, ReplacingTextureIndex, BankIndex, Blend).  Assignment is the implicit 6-dword copy;
// the by-value pair is built in the _Push_heap slot through `mov eax,esp`, the key stored, then
// `add eax,4` and the entry copied word by word: CTextureMorphEntry has a user-provided inline copy
// constructor written as an assignment (`*this = other`; a member-initialiser list folds the +4 into
// each store instead), the pair's own copy constructor is implicit.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
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

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare pred);   // 0x00819B10

void __fastcall _Adjust_heap(TEntry* first, long hole, long bottom, TEntry value, TCompare pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}