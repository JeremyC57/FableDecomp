// std::_Push_heap(std::pair<unsigned long,CRandomAppearanceMorph::CTextureMorph>* first, int hole, int top, std::pair<unsigned long,CRandomAppearanceMorph::CTextureMorph> val, CKeyPairCompareLess<unsigned long,CRandomAppearanceMorph::CTextureMorph> pred) -- percolate a keyed texture morph up a max-heap
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00819C30, 106 bytes (/O2). Called from the matching _Adjust_heap @ 0x0081AC20.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 12-byte element
// (`lea edx,[eax+eax*2]`, stride 12): Ego_r.pdb CRandomAppearanceMorph::CTextureMorph is 8 bytes
// (unsigned long NewTexture, GroupID), so pair<unsigned long, CTextureMorph> is 12 bytes, as labelled.
// Element assignment is the implicit three-dword copy; CKeyPairCompareLess compares the keys
// UNSIGNED (`cmp ebp,ebx ; jae`). /Gr: first=ecx, hole=edx; `ret 14h` = top + 12-byte pair + the
// empty comparator.
namespace N00819c30 {
namespace CRandomAppearanceMorph {
struct CTextureMorph {
    unsigned long NewTexture;                             // +0x0
    unsigned long GroupID;                                // +0x4
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
typedef pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> Elem;
typedef CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}