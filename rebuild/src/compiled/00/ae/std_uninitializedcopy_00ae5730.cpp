// std::__uninitialized_copy(const CUnprovenElement_00ae5730* first, const CUnprovenElement_00ae5730* last, CUnprovenElement_00ae5730* result, const __false_type&) -- copy-construct [first, last) of 16-byte elements into raw storage at result, return the end
// Leaf: ??$__uninitialized_copy@PBUCUnprovenElement_00ae5730@@PAU1@@std@@YIPAUCUnprovenElement_00ae5730@@PBU1@0PAU1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE5730, 62 bytes, /Gr `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity: the function is STLport 4.6 `__uninitialized_copy` (same shape as the landed 0x00ADA7E0): its only caller is
//   the vector copy constructor at 0x00AE5C60 (allocate size*16, then copy x._M_start..x._M_finish into _M_start).
// Types: unproven -- the copy constructor's only caller 0x00AE69D0 is itself unreferenced, so nothing in retail names the
//   16-byte element; CUnprovenElement_00ae5730 is a placeholder whose implicit copy is the four-dword block copy.
inline void* operator new(unsigned int, void* __p) { return __p; }
struct CUnprovenElement_00ae5730 {
    long _M_unproven0;
    long _M_unproven1;
    long _M_unproven2;
    long _M_unproven3;
};
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CUnprovenElement_00ae5730* __fastcall __uninitialized_copy(const CUnprovenElement_00ae5730*, const CUnprovenElement_00ae5730*, CUnprovenElement_00ae5730*, const __false_type&);
}   // namespace std