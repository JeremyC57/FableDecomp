// std::_Push_heap(std::pair<unsigned long,C3DHelperPoint>* first, int hole, int top, std::pair<unsigned long,C3DHelperPoint> val, CKeyPairCompareLess<unsigned long,C3DHelperPoint> pred) -- percolate a CRC-keyed helper point up a max-heap ordered by the unsigned key
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00A31380, 144 bytes incl. int3 padding (/O2). Called from the landed
// _Adjust_heap @ 0x00A32440 (same instantiation).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 20-byte element
// (`lea edx,[eax+eax*4]`): Ego_r.pdb C3DHelperPoint = C3DVector Position + long BoneIndex (16 bytes),
// so pair<unsigned long, C3DHelperPoint> is 20 bytes, as labelled. Assignment is the implicit
// five-dword copy (C3DVector has a memberwise user copy constructor but no user operator=); the key
// test is UNSIGNED (`jae`). /Gr: first=ecx, hole=edx; `ret 1Ch` = top + 20-byte pair + comparator.
namespace N00a31380 {
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct C3DHelperPoint {
    C3DVector Position;                                   // +0x0
    long BoneIndex;                                       // +0xC
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
typedef pair<unsigned long, C3DHelperPoint> Elem;
typedef CKeyPairCompareLess<unsigned long, C3DHelperPoint> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}