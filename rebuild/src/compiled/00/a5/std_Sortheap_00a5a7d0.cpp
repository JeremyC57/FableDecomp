// std::_Sort_heap(float* first, float* last, std::less<float> pred) -- order a heap of floats by repeatedly popping the front to the back
// Flags: /Gr
// Leaf: _Sort_heap
// Retail: Fable.exe @ 0x00A5A7D0, 144 bytes (/O2). Callee: STLport __push_heap<float*,int,float,less<float>>
// 0x00A586D0 (`__fastcall`, `ret 0Ch`; catalog label `_Push_heap<pair<float,CCharString>*,...>` is propagated).
// Shape: the same STLport `sort_heap` source as 0x00777EE0 (the integer twin): `for (; 1 < _Last - _First;
// --_Last)` popping through the inline __pop_heap_aux -> __pop_heap -> __adjust_heap (`fld ; fcomp ; test ah,5 ;
// jp` is `_Pred(*(_First + _Idx), *(_First + (_Idx - 1)))` = less<float>), only __push_heap out of line. The
// popped value is a float, homed in the comparator's dead argument slot across the sift.
namespace std {
template <class _Ty> struct less {
    bool operator()(const _Ty& _Left, const _Ty& _Right) const { return _Left < _Right; }
};
template <class _RanIt, class _Diff, class _Ty, class _Pr>
void __fastcall _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred);   // 0x00A586D0
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
template void __fastcall _Sort_heap<float*, less<float> >(float*, float*, less<float>);
}