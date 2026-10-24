// std::_Push_heap(std::pair<unsigned __int64,SList8_00517720>* first, int hole, int top, std::pair<unsigned __int64,SList8_00517720> val, CKeyPairCompareLess<unsigned __int64,SList8_00517720> pred) -- percolate a 64-bit-keyed list up a max-heap, then destroy the by-value list
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00A820E0, 152 bytes (/O2). Called from _Adjust_heap @ 0x00A82BE0. Callees: the
// 8-byte list's operator= 0x00517720 (catalog "std::list::operator=") and its destructor 0x005127A0
// (walks and frees the nodes, then the head).
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 16-byte element
// (`shl eax,4`): an UNSIGNED 64-bit key +0 (high word compared first: `ja done ; jb move`, then the
// low word `jae done`) and an 8-byte list {head, size} +8 -- the same list member the landed
// __copy_backward of this TU (0x00A821C0) assigns through 0x00517720. The implicit pair operator= is
// the two key words then the list's out-of-line operator=; the by-value list is destroyed at the end.
// Label correction: the propagated pair<unsigned __int64, CRuntimeScriptedEntityPersist::CPersistInfo>
// is 24 bytes (UID + CCharString, the twin at 0x00CB93E0); the list's value type is not pinned here, so
// the member keeps the TU's descriptive name.
// /Gr: first=ecx, hole=edx; `ret 18h` = top + 16-byte pair + the empty comparator.
namespace N00a820e0 {
struct SList8_00517720 {                                  // 8 bytes
    void* m_pHead;
    long  m_size;
    SList8_00517720& operator=(const SList8_00517720& other);   // 0x00517720
    ~SList8_00517720();                                          // 0x005127A0
};
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
typedef pair<unsigned __int64, SList8_00517720> Elem;
typedef CKeyPairCompareLess<unsigned __int64, SList8_00517720> Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}