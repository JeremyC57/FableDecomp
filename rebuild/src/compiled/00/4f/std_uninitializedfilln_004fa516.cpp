// std::uninitialized_fill_n(CCardDef* first, unsigned int n, const CCardDef& x) -- copy-construct n copies of x into raw storage; returns the end
// Flags: /Gr
// Leaf: ??$uninitialized_fill_n@PAVCCardDef@@IV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FA516, 34 bytes, __fastcall (first ecx, n edx, &x on the stack), `ret 4`.
// Identity (proven): reloc_bindings ??$uninitialized_fill_n@PAVCCardDef@@IV1@@std@@YIPAVCCardDef@@PAV1@IABV1@@Z; its _Construct 0x004EC08A is
//   bound ??$_Construct@VCCardDef@@@std@@ (0x44 stride); caller the landed vector<CCardDef>::_M_fill_insert 0x004FA277.
// Callee: _Construct(T*, const T&) 0x004EC08A (__fastcall, per element).
// Shape (family K125, representative 0x004ED437): STLport 4.6 `uninitialized_fill_n` -> `__uninitialized_fill_n(..., __false_type)`
//   loop inline (size TU: __forceinline), the element construct out of line, &x re-read from its argument slot each pass.
#pragma optimize("s", on)
class CCardDef {                                         // retail 0x44 bytes; opaque here
public:
    unsigned char _data[0x44];
};
namespace std {
struct __false_type { };
template <class T> void __fastcall _Construct(T* p, const T& val);                     // 0x004EC08A
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
template CCardDef* __fastcall uninitialized_fill_n<CCardDef*, unsigned int, CCardDef>(CCardDef*, unsigned int, const CCardDef&);
}