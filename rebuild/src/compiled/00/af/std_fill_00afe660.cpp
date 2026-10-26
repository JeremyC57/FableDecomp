// std::fill(CArray<long>* first, CArray<long>* last, const CArray<long>& value) -- (CArray<long>) STLport fill: assign value to every element of [first, last) through the element's assignment (CArray<long>'s implicit operator= inline, i.e. vector<long>::operator= 0x00454886)
// Leaf: ??$fill@PAV?$CArray@J@@V1@@std@@YIXPAV?$CArray@J@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE660, 37 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx, the value
//   reference on the stack), `ret 4`.
// Identity (probable): std::fill<CArray<long>*, CArray<long> >; evidence: the only caller 0x00AFE7C0 is the _M_fill_insert of a vector of 12-byte elements (stride 0xC; reached from the resize 0x00AFE910 and CTriangleStripifier2 0x00F3E6B0); the element assignment is 0x00454886, the 4-byte-element vector assignment that the def operator= bodies use for their Ego_r vector<long> members; of the Ego_r vector-of-vector types only vector<CArray<long> > has a long element (no vector<vector<long> >); the PDB fill set has no CArray<long> instantiation (debug-build sets are incomplete) (stride 0xC asserted)
// Label: the manifest's "fill<CArray<unsigned_short>*,CArray<unsigned_short>_>" is propagated from 0x00F3B850 (five byte-similar fill bodies; retail has no identical-code folding,
//   so each address is its own element type).
// Callee: std::vector<long>::operator= (??4?$vector@JV?$allocator@J@std@@@std@@QAEAAV01@ABV01@@Z, a PDB spelling; CArray<long>'s implicit operator= is inline) -- the def assignments that call it copy Ego_r vector<long> members (CMaterialDef's five particle-effect vectors, CHitLocationsDef::HitLocations) 0x00454886.
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
template void __fastcall std::fill<CArray<long>*, CArray<long> >(CArray<long>*, CArray<long>*, const CArray<long>&);