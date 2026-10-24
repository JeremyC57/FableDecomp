// std::_Push_heap(std::pair<long,unsigned long>* first, int hole, int top, std::pair<long,unsigned long> val, std::less<std::pair<long,unsigned long> > pred) -- percolate a pair up a max-heap ordered by pair operator<
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00AE5AF0, 87 bytes (/O2, rotated loop).
// STLport `__push_heap` (catalog `_Push_heap`) with the default `less<pair>` predicate: the lexicographic
// `a.first < b.first || !(b.first < a.first) && a.second < b.second` gives the signed
// `cmp edi,edx ; jg move ; jl done` on the long key and the unsigned `cmp [..+4],ebp ; jae done`
// on the second word. /Gr: first=ecx, hole=edx; `ret 10h` = top + 8-byte pair + empty predicate.
namespace N00ae5af0 {
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> inline bool operator<(const pair<K, V>& l, const pair<K, V>& r)
{
    return l.first < r.first || !(r.first < l.first) && l.second < r.second;
}
template<class T> struct less { bool operator()(const T& l, const T& r) const { return l < r; } };
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<long, unsigned long> Elem;
template void _Push_heap<Elem*, int, Elem, less<Elem> >(Elem*, int, int, Elem, less<Elem>);
}