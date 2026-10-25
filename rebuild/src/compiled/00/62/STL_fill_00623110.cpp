// _STL::fill(_STL::pair<float, CWideString>* first, _STL::pair<float, CWideString>* last, const _STL::pair<float, CWideString>& value) -- assign one (float, wide string) pair to every element of a range
// Leaf: ??$fill@PAU?$pair@MVCWideString@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623110, 46 bytes, `__fastcall` (first ecx, last edx, value pointer
//   on the stack), `ret 4`.
// Identity: 8-byte stride = {4-byte member +0 copied by register, member +4 assigned through 0x0099B7D0
//   CWideString::operator= (manifest engine_api, the string family of 0x0099B510 ~CWideString)}: Ego_r.pdb has
//   _Pair_base<float, CWideString>, the only (4-byte, CWideString) pair. The manifest's "pair<float, CCharString>"
//   has the wrong string type. One caller, 0x00624F1F.
// Callees: CWideString::operator=(const CWideString&) @ 0x0099B7D0.
// Shape: STLport fill with pair's implicit operator= inline: the float is copied with integer moves (no x87 for a
//   plain member copy), then the string assignment; &val.second hoisted.

class CWideString {
public:
    void* PStringData;
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
};
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef _STL::pair<float, CWideString> CFloatWideStringPair;

namespace _STL {
// STLport 4.6 fill; the algorithm templates are __fastcall here (first/last in ecx/edx, callee pops the value pointer).
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall _STL::fill<CFloatWideStringPair*, CFloatWideStringPair>(CFloatWideStringPair*, CFloatWideStringPair*, const CFloatWideStringPair&);