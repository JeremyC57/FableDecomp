// _global::_Adjust_heap(pair<long,CRandomAppearanceMorph::CSkeletalMorph>*, long, long, pair<long,CRandomAppearanceMorph::CSkeletalMorph>, CKeyPairCompareLess<long,CRandomAppearanceMorph::CSkeletalMorph>) -- heap sift-down of random-appearance skeletal morphs by group key, then _Push_heap @ 0x0086A7F0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B070, 171 bytes.
// Callee: 0x0086A7F0 _Push_heap (manifest: _Push_heap<pair<long,CRandomAppearanceMorph::CSkeletalMorph>*,...>,
// the callee's own label; this row's propagated label pair<EHeroMorphType,CTextureMorphEntry> is
// wrong -- the element is 16 bytes).  Link-time __fastcall: first in ecx, hole in edx, then bottom,
// the 16-byte pair by value and the 4-byte comparator; `ret 0x18`.
// Element (16 bytes): long key +0, CRandomAppearanceMorph::CSkeletalMorph +4 (PDB, 12 bytes: long
// GroupID, CDefString Name, bool AllowVariation).  Assignment is the implicit 4-dword copy; the
// by-value pair is built in the _Push_heap slot through `mov eax,esp`, the key stored, then
// `add eax,4` and the morph copied word by word: the morph's inline copy constructor is written as
// an assignment (`*this = other`), as in the sibling 0x0081AA50.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct CDefString {
    long TablePos;
};

struct CSkeletalMorph {                         // CRandomAppearanceMorph::CSkeletalMorph
    long GroupID;                               // +0x0
    CDefString Name;                            // +0x4
    bool AllowVariation;                        // +0x8
    CSkeletalMorph(const CSkeletalMorph& other) { *this = other; }
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<long, CSkeletalMorph> TEntry;

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CSkeletalMorph> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare pred);   // 0x0086A7F0

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