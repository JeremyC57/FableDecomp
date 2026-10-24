// std::_Push_heap(std::pair<unsigned long,C3DVector>* first, int hole, int top, std::pair<unsigned long,C3DVector> val, CKeyPairCompareLess<unsigned long,C3DVector> pred) -- percolate a keyed 3D vector up a max-heap ordered by the unsigned key
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00769470, 118 bytes (/O2). Called from the landed _Adjust_heap @ 0x00769D50
// (same instantiation: 16-byte (unsigned long key, C3DVector) pairs).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 16-byte element
// (`shl edx,4`); element assignment is the implicit four-dword copy, the key test an UNSIGNED `jae`.
// C3DVector keeps the memberwise user copy constructor the caller's model needs; it has no user
// operator=, so assignment stays a plain dword copy. Label correction: the propagated
// pair<long, CRandomAppearanceMorph::CSkeletalMorph> is the SIGNED-key byte twin at 0x0086A7F0.
// /Gr: first=ecx, hole=edx; `ret 18h` = top + 16-byte pair + the empty comparator.
namespace N00769470 {
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
template<class K, class V> struct pair {
    K first; V second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
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
typedef pair<unsigned long, C3DVector> Elem;
typedef CKeyPairCompareLess<unsigned long, C3DVector> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}