// std::_Sort_heap(std::pair<float,EConversationAttitude>* first, std::pair<float,EConversationAttitude>* last, CKeyPairCompareLess<float,EConversationAttitude> pred) -- order a heap of (score, attitude) pairs by repeatedly popping the front to the back
// Flags: /Gr
// Leaf: _Sort_heap
// Retail: Fable.exe @ 0x004D15A0, 96 bytes (/O2). Callee: STLport __adjust_heap<pair<float,
// EConversationAttitude>*,int,pair<...>,CKeyPairCompareLess<float,EConversationAttitude>> 0x004D04F0
// (`__fastcall`: first ecx, hole edx, bottom + the 8-byte value + the comparator on the stack).
// Shape: STLport `sort_heap` (the 0x00777EE0 source): `for (; 1 < _Last - _First; --_Last)` popping
// through the inline __pop_heap_aux -> __pop_heap (`*_Dest = *_First`, the implicit assignment's two dword copies)
// with __adjust_heap out of line from hole 0. The popped value `_Ty(*(_Last - 1))` goes straight into the
// argument area through the pair's memberwise user copy constructor (the float through `fld`/`fstp`, the
// attitude as a dword) -- the same body as the eight landed 8-byte pair rows (0x004CD130 ...) with a float key.
// Label correction: the catalog's `_Sort_heap<std::pair<long,unsigned_long>*>` is propagated -- the key is a
// float (`fld`) and the callee is the pair<float,EConversationAttitude> _Adjust_heap with a by-value
// CKeyPairCompareLess comparator (`ret 4`).
enum EConversationAttitude { CONVERSATION_ATTITUDE_FORCE_DWORD = 0x7fffffff };
template <class K, class V> struct CKeyPairCompareLess { };
namespace std {
template <class _Ty1, class _Ty2> struct pair {
    _Ty1 first;
    _Ty2 second;
    pair(const pair& _Right) : first(_Right.first), second(_Right.second) { }
};
template <class _RanIt, class _Diff, class _Ty, class _Pr>
void __fastcall _Adjust_heap(_RanIt _First, _Diff _Hole, _Diff _Bottom, _Ty _Val, _Pr _Pred);   // 0x004D04F0
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
typedef pair<float, EConversationAttitude> Elem;
template void __fastcall _Sort_heap<Elem*, CKeyPairCompareLess<float, EConversationAttitude> >(Elem*, Elem*, CKeyPairCompareLess<float, EConversationAttitude>);
}