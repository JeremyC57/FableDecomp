// std::_Sort_heap(pair<long,CSkeletalMorph>* first, pair<long,CSkeletalMorph>* last, CKeyPairCompareLess<long,CSkeletalMorph> pred) -- Dinkumware _Sort_heap over random-appearance skeletal morphs keyed by group: pop the root to the back until one element remains
// Leaf: _Sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086C950, length 162 bytes, `ret 4`.
// Identity: catalog `_Sort_heap<pair<long,CRandomAppearanceMorph::CSkeletalMorph>*,CKeyPairCompareLess<...>>`,
// consistent with the bytes: the only callee is the landed `_Adjust_heap` @ 0x0086B070 of that instantiation
// (Ego_r.pdb CSkeletalMorph, 12 bytes: GroupID, CDefString Name, bool AllowVariation); element model reused.
// Callees: _Adjust_heap @ 0x0086B070 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 18h`).
// Shape: /Gr Dinkumware TU with _Pop_heap_0 / _Pop_heap inline and the loop calling _Pop_heap_0 directly (16-byte
// stride: `and -16 ; cmp 16`). `_Ty(*(_Last - 1))` is a frame temporary: the key stays in ecx, the morph is copied
// as one 12-byte block through its own address (`lea edx,[esi+4]`: the morph's copy constructor is written as an
// assignment, `*this = other`); `*_Dest = *_First` is the implicit assignment (four dword moves through ebp), and
// the value argument gets the key, then the morph block through `add eax,4`.
class CDefString { public:
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

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CSkeletalMorph> TCompare;

void __fastcall _Adjust_heap(TEntry* first, long hole, long bottom, TEntry value, TCompare pred);   // 0x0086B070

inline void _Pop_heap(TEntry* first, TEntry* last, TEntry* dest, TEntry value, TCompare pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}

inline void _Pop_heap_0(TEntry* first, TEntry* last, TCompare pred, TEntry*)
{
    _Pop_heap(first, last - 1, last - 1, TEntry(*(last - 1)), pred, (long*)0);
}

void __fastcall _Sort_heap(TEntry* first, TEntry* last, TCompare pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (TEntry*)0);
}