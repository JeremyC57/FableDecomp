// std::_Push_heap(std::pair<EExpressionType,CExpressionSet::CEntry>* first, int hole, int top, std::pair<EExpressionType,CExpressionSet::CEntry> val, CKeyPairCompareLess<EExpressionType,CExpressionSet::CEntry> pred) -- percolate an expression-table entry up a max-heap ordered by expression type
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x0086F100, 106 bytes (/O2). Called from the landed _Adjust_heap @ 0x0086F450
// (same instantiation, the CExpressionSet table).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 12-byte element:
// EExpressionType key +0, CExpressionSet::CEntry +4 (Ego_r.pdb, 8 bytes: Type, AnimIndex). The
// enum key compares SIGNED (`cmp ebp,ebx ; jge`); element assignment is the implicit three-dword
// copy. The propagated label pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> is the
// unsigned-key byte twin (0x0086A770); this body's signed compare and its caller name it.
// /Gr: first=ecx, hole=edx; `ret 14h` = top + 12-byte pair + the empty comparator.
namespace N0086f100 {
enum EExpressionType { EXPRESSION_TYPE_NONE };
namespace CExpressionSet {
struct CEntry {
    EExpressionType Type;                                 // +0x0
    long AnimIndex;                                       // +0x4
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
typedef pair<EExpressionType, CExpressionSet::CEntry> Elem;
typedef CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}