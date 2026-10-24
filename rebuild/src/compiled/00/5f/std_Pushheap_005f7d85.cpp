// std::_Push_heap(std::pair<long,long>* first, int hole, int top, std::pair<long,long> val, CKeyPairCompareLess pred) -- percolate a keyed 8-byte pair up a binary heap
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x005F7D85, 62 bytes, size-optimised region (unaligned, `jmp` into the loop test).
// STLport `__push_heap` (catalog `_Push_heap`): `for (idx = (hole-1)/2; hole > top && pred(first[idx], val); idx = (hole-1)/2)
// { first[hole] = first[idx]; hole = idx; } first[hole] = val;` under /Gr (first=ecx, hole=edx,
// top/val/pred on the stack, `ret 10h` = top + 8-byte pair + 4-byte empty comparator).
// The element stride is 8 and the key compare is signed (`cmp [ecx+eax*8],edi ; jge`): the
// propagated label `pair<unsigned long,CSoundPair>` cannot be right (CSoundPair is 8 bytes in the
// PDB and the key would compare unsigned), so the element is modelled as a signed key + one dword.
// `#pragma optimize("s", on)` keeps the unrotated loop (`lea eax,[esi-1] ; jmp test`).
#pragma optimize("s", on)
namespace N005f7d85 {
template<class K, class V> struct pair { K first; V second; };
struct CKeyPairCompareLess {
    bool operator()(const pair<long, long>& a, const pair<long, long>& b) const { return a.first < b.first; }
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
template void _Push_heap<pair<long, long>*, int, pair<long, long>, CKeyPairCompareLess>(pair<long, long>*, int, int, pair<long, long>, CKeyPairCompareLess);
}