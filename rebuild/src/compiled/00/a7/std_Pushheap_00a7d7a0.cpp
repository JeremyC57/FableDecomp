// std::_Push_heap(std::pair<unsigned __int64,CANavQuadTreeNode*>* first, int hole, int top, std::pair<unsigned __int64,CANavQuadTreeNode*> val, CKeyPairCompareLess<unsigned __int64,CANavQuadTreeNode*> pred) -- percolate a 64-bit-keyed nav quad-tree entry up a max-heap
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00A7D7A0, 129 bytes (/O2). Called from _Adjust_heap @ 0x00A7F1A0 (byte twin of 0x00A7D6F0, a second u64-keyed map of the TU).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 16-byte element
// (`shl edx,4`) whose key is an UNSIGNED 64-bit value: the compare tests the high word first
// (`cmp [..+4],ebp ; ja done ; jb move`) and then the low word (`jae done`). The element is the
// CVectorMap<unsigned __int64, CANavQuadTreeNode*> entry of this TU (its LowerBound/UpperBound are
// 0x00A7DC60/0x00A7DCC0/0x00A7DF60): an 8-aligned pair, so the pointer is followed by a padding word
// and the trivial assignment copies all four dwords. Label correction: the propagated
// pair<pair<long,long>, CEngineTextureDecal> is 28 bytes (the real one is 0x00BEE420); a pair<long,long>
// key would compare +0 signed first.
// /Gr: first=ecx, hole=edx; `ret 18h` = top + 16-byte pair + the empty comparator.
namespace N00a7d7a0 {
class CANavQuadTreeNode;
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
typedef pair<unsigned __int64, CANavQuadTreeNode*> Elem;
typedef CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}