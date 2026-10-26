// std::fill(CArray<unsigned short>* first, CArray<unsigned short>* last, const CArray<unsigned short>& value) -- (CArray<unsigned short>) STLport fill: assign value to every element of [first, last) through the element's assignment (CArray<unsigned short>'s implicit operator= inline, i.e. vector<unsigned short>::operator= 0x00F3B640)
// Leaf: ??$fill@PAV?$CArray@G@@V1@@std@@YIXPAV?$CArray@G@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B850, 37 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx, the value
//   reference on the stack), `ret 4`.
// Identity (probable): std::fill<CArray<unsigned short>*, CArray<unsigned short> >; evidence: the only caller 0x00F3F960 is the _M_fill_insert of a vector of 12-byte elements (stride 0xC; reached from CTriangleStripifier2 0x00F3E6B0 and the resize 0x00F3FAF0); the element assignment 0x00F3B640 is a full vector assignment over 2-byte elements (`sar 1`, malloc/memmove/free); Ego_r has vector<CArray<unsigned short> > and the Ego_r/FableWin.pdb name sets hold ??$fill@PAV?$CArray@G@@V1@@std@@YIXPAV?$CArray@G@@0ABV1@@Z (stride 0xC asserted)
// Label: the manifest's "fill<CArray<unsigned_short>*,CArray<unsigned_short>_>" names this row's element correctly
// Callee: std::vector<unsigned short>::operator= (??4?$vector@GV?$allocator@G@std@@@std@@QAEAAV01@ABV01@@Z, a PDB spelling; CArray<unsigned short>'s implicit operator= is inline) 0x00F3B640.
// Body: STLport 4.6 `for ( ; __first != __last; ++__first) *__first = __val;` (explicit instantiation of the inline template).
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // out of line
};
}   // namespace std
template <class _Tp> class CArray : public std::vector<_Tp> {   // Fable's CArray<T>: a std::vector<T> (Ego_r 16 = the donor vector)
};
namespace std {
template <class _ForwardIter, class _Tp>
inline void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}   // namespace std
template void __fastcall std::fill<CArray<unsigned short>*, CArray<unsigned short> >(CArray<unsigned short>*, CArray<unsigned short>*, const CArray<unsigned short>&);