// std::fill(CWideString* first, CWideString* last, const CWideString& value) -- (CWideString) STLport fill: assign value to every element of [first, last) through the element's assignment (CWideString::operator= 0x0099B7D0)
// Leaf: ??$fill@PAVCWideString@@V1@@std@@YIXPAVCWideString@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040E0D0, 37 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx, the value
//   reference on the stack), `ret 4`.
// Identity (proven): std::fill<CWideString*, CWideString >; evidence: the only caller is the landed std::vector<CWideString>::_M_fill_insert 0x004126E0 (identity there: stride 4 with CWideString's copy constructor 0x0099B720, this assignment 0x0099B7D0 and destructor 0x0099B510); stride 4 = sizeof Ego_r CWideString; the Ego_r/FableWin.pdb name sets hold ??$fill@PAVCWideString@@V1@@std@@YIXPAVCWideString@@0ABV1@@Z (stride 0x4 asserted)
// Label: the manifest's "fill<CArray<unsigned_short>*,CArray<unsigned_short>_>" is propagated from 0x00F3B850 (five byte-similar fill bodies; retail has no identical-code folding,
//   so each address is its own element type).
// Callee: CWideString::operator=(const CWideString&) (??4CWideString@@QAEABV0@ABV0@@Z, the PDB spelling: it returns const CWideString&) 0x0099B7D0.
// Body: STLport 4.6 `for ( ; __first != __last; ++__first) *__first = __val;` (explicit instantiation of the inline template).
class CWideStringData;
class CWideString {                                     // 4 bytes (Ego_r)
public:
    CWideStringData* PStringData;                       // +0x00
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
namespace std {
template <class _ForwardIter, class _Tp>
inline void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}   // namespace std
template void __fastcall std::fill<CWideString*, CWideString >(CWideString*, CWideString*, const CWideString&);