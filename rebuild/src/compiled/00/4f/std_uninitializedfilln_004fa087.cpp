// std::uninitialized_fill_n(CCardPositionsDef* first, unsigned int n, const CCardPositionsDef& x) -- copy-construct n copies of x into raw storage; returns the end
// Flags: /Gr
// Leaf: ??$uninitialized_fill_n@PAVCCardPositionsDef@@IV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FA087, 34 bytes, __fastcall (first ecx, n edx, &x on the stack), `ret 4`.
// Identity (proven): reloc_bindings ??$uninitialized_fill_n@PAVCCardPositionsDef@@IV1@@std@@YIPAVCCardPositionsDef@@PAV1@IABV1@@Z; its _Construct 0x004EBDF1 is
//   bound ??$_Construct@VCCardPositionsDef@@@std@@ (0x48 stride); caller the landed vector<CCardPositionsDef>::_M_fill_insert 0x004F9DF8.
// Callee: _Construct(T*, const T&) 0x004EBDF1 (__fastcall, per element).
// Shape (family K125, representative 0x004ED437): STLport 4.6 `uninitialized_fill_n` -> `__uninitialized_fill_n(..., __false_type)`
//   loop inline (size TU: __forceinline), the element construct out of line, &x re-read from its argument slot each pass.
#pragma optimize("s", on)
class CCardPositionsDef {                                // retail 0x48 bytes; opaque here
public:
    unsigned char _data[0x48];
};
namespace std {
struct __false_type { };
template <class T> void __fastcall _Construct(T* p, const T& val);                     // 0x004EBDF1
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
template CCardPositionsDef* __fastcall uninitialized_fill_n<CCardPositionsDef*, unsigned int, CCardPositionsDef>(CCardPositionsDef*, unsigned int, const CCardPositionsDef&);
}