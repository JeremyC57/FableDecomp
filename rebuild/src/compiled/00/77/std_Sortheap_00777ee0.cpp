// std::_Sort_heap(long* first, long* last, std::less<long> pred) -- order a heap of 4-byte signed integers by repeatedly popping the front to the back
// Flags: /Gr
// Leaf: _Sort_heap
// Retail: Fable.exe @ 0x00777EE0, 123 bytes (/O2). Callee: STLport __push_heap<long*,int,long,less<long>> (catalog `_Push_heap<float*,...>`, propagated)
// 0x00776750 (`__fastcall`, first ecx, hole edx, top/value/pred on the stack, `ret 0Ch`).
// Shape: STLport `sort_heap` (catalog name `_Sort_heap`; the identifiers keep the catalog's spelling):
// `while (last - first > 1) pop_heap(first, last--, comp);` with STLport's pop_heap (no count test of its own)
// -> __pop_heap_aux (here _Pop_heap_0) -> __pop_heap (`*result = *first`) -> __adjust_heap, all inline
// (`__forceinline`): the sift-down to the larger child (`secondChild = 2 * hole + 2`, `if (comp(first
// [secondChild], first[secondChild - 1])) --secondChild;`, the lone-child step through `secondChild - 1`)
// and only __push_heap out of line (top 0 pushed as a constant). A Dinkumware-style pop_heap with its own
// `1 < last - first` test keeps the element count in a register (`sar ; cmp 1`) instead of retail's
// byte-count test (`and -4 ; cmp 4`), and `bottom - 1` in the lone-child step swaps eax/ecx.
// Label correction: the catalog's `_Sort_heap<float*,std::less<float>>` is propagated -- both the inline sift
// (`cmp ebp,[..] ; jge`) and the callee 0x00776750 compare as SIGNED INTEGERS (00A5A7D0 is the float twin
// with fcomp), so the element is a 4-byte signed integer (modelled as long).
namespace std {
template <class _Ty> struct less {
    bool operator()(const _Ty& _Left, const _Ty& _Right) const { return _Left < _Right; }
};
template <class _RanIt, class _Diff, class _Ty, class _Pr>
void __fastcall _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred);   // 0x00776750
template <class _RanIt, class _Diff, class _Ty, class _Pr> __forceinline
void _Adjust_heap(_RanIt _First, _Diff _Hole, _Diff _Bottom, _Ty _Val, _Pr _Pred)
{
    _Diff _Top = _Hole;
    _Diff _Idx = 2 * _Hole + 2;
    for (; _Idx < _Bottom; _Idx = 2 * _Idx + 2) {
        if (_Pred(*(_First + _Idx), *(_First + (_Idx - 1))))
            --_Idx;
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    if (_Idx == _Bottom) {
        *(_First + _Hole) = *(_First + (_Idx - 1));
        _Hole = _Idx - 1;
    }
    _Push_heap(_First, _Hole, _Top, _Val, _Pred);
}
template <class _RanIt, class _Diff, class _Ty, class _Pr> __forceinline
void _Pop_heap(_RanIt _First, _RanIt _Last, _RanIt _Dest, _Ty _Val, _Pr _Pred, _Diff*)
{
    *_Dest = *_First;
    _Adjust_heap(_First, _Diff(0), _Diff(_Last - _First), _Val, _Pred);
}
template <class _RanIt, class _Ty, class _Pr> __forceinline
void _Pop_heap_0(_RanIt _First, _RanIt _Last, _Pr _Pred, _Ty*)
{
    _Pop_heap(_First, _Last - 1, _Last - 1, _Ty(*(_Last - 1)), _Pred, (int*)0);
}
template <class _RanIt, class _Pr>
void __fastcall _Sort_heap(_RanIt _First, _RanIt _Last, _Pr _Pred)
{
    for (; 1 < _Last - _First; --_Last)
        _Pop_heap_0(_First, _Last, _Pred, _First);
}
template void __fastcall _Sort_heap<long*, less<long> >(long*, long*, less<long>);
}