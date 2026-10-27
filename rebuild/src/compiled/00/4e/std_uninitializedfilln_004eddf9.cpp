// std::uninitialized_fill_n(CPrizeScoreDef* first, unsigned int n, const CPrizeScoreDef& x) -- copy-construct n copies of x into raw storage; returns the end
// Flags: /Gr
// Leaf: ??$uninitialized_fill_n@PAVCPrizeScoreDef@@IV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDDF9, 34 bytes, __fastcall (first ecx, n edx, &x on the stack), `ret 4`.
// Identity (proven): reloc_bindings ??$uninitialized_fill_n@PAVCPrizeScoreDef@@IV1@@std@@YIPAVCPrizeScoreDef@@PAV1@IABV1@@Z; its _Construct 0x004EAC30 is
//   bound ??$_Construct@VCPrizeScoreDef@@@std@@ (0x30 stride); caller the landed vector<CPrizeScoreDef>::_M_fill_insert 0x004EDB3E.
// Callee: _Construct(T*, const T&) 0x004EAC30 (__fastcall, per element).
// Shape (family K125, representative 0x004ED437): STLport 4.6 `uninitialized_fill_n` -> `__uninitialized_fill_n(..., __false_type)`
//   loop inline (size TU: __forceinline), the element construct out of line, &x re-read from its argument slot each pass.
#pragma optimize("s", on)
class CPrizeScoreDef {                                   // retail 0x30 bytes; opaque here
public:
    unsigned char _data[0x30];
};
namespace std {
struct __false_type { };
template <class T> void __fastcall _Construct(T* p, const T& val);                     // 0x004EAC30
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template CPrizeScoreDef* __fastcall uninitialized_fill_n<CPrizeScoreDef*, unsigned int, CPrizeScoreDef>(CPrizeScoreDef*, unsigned int, const CPrizeScoreDef&);
}