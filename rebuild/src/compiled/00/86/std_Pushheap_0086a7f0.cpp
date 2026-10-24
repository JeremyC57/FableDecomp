// std::_Push_heap(std::pair<long,CRandomAppearanceMorph::CSkeletalMorph>* first, int hole, int top, std::pair<long,CRandomAppearanceMorph::CSkeletalMorph> val, CKeyPairCompareLess<long,CRandomAppearanceMorph::CSkeletalMorph> pred) -- percolate a random-appearance skeletal morph up a max-heap ordered by its group key
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x0086A7F0, 118 bytes (/O2). Called from the landed _Adjust_heap @ 0x0086B070.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 16-byte element:
// long key +0, CRandomAppearanceMorph::CSkeletalMorph +4 (Ego_r.pdb, 12 bytes: long GroupID,
// CDefString Name, bool AllowVariation). The morph's operator= is implicit and trivial, so VC copies
// the element as four dwords (the bool's padding included); the key test is SIGNED (`jge`).
// /Gr: first=ecx, hole=edx; `ret 18h` = top + 16-byte pair + the empty comparator.
namespace N0086a7f0 {
struct CDefString {
    long TablePos;
};
namespace CRandomAppearanceMorph {
struct CSkeletalMorph {
    long GroupID;                                         // +0x0
    CDefString Name;                                      // +0x4
    bool AllowVariation;                                  // +0x8
    CSkeletalMorph(const CSkeletalMorph& other) { *this = other; }
};
}
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<long, CRandomAppearanceMorph::CSkeletalMorph> Elem;
typedef CKeyPairCompareLess<long, CRandomAppearanceMorph::CSkeletalMorph> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}