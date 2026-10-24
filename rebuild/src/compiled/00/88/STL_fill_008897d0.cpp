// _STL::fill(CBlendedEnvironmentTheme* first, CBlendedEnvironmentTheme* last, const CBlendedEnvironmentTheme& val) -- assign val to every blended environment theme of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008897D0, length 48 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `fill<CArray<unsigned_short>*,...>` is propagated from a byte twin. One caller, 0x0088A284,
//   the vector<CBlendedEnvironmentTheme> fill-insert of the environment TU, which also calls the uninitialized_fill_n
//   0x00889800 (copy constructor 0x008894A0) on the same 0x1F4-byte element.
// Callees: NEnvironmentInternal::CBlendedEnvironmentTheme::operator=(const CBlendedEnvironmentTheme&) @ 0x006B5DF0 (landed).
// Shape: STLport 4.x `fill(first, last, val)` in a /Gr TU (`__fastcall`: first in ecx -> esi, last in edx -> edi, the
//   value reference on the stack -> ebx, loaded after the empty-range test); one out-of-line assignment per element,
//   stride 0x1F4.
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {                // 500 bytes (retail 12-byte CArrays)
public:
    unsigned char _body[0x1F4];
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);   // 0x006B5DF0
};
}

namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill<NEnvironmentInternal::CBlendedEnvironmentTheme*, NEnvironmentInternal::CBlendedEnvironmentTheme>(
    NEnvironmentInternal::CBlendedEnvironmentTheme*, NEnvironmentInternal::CBlendedEnvironmentTheme*,
    const NEnvironmentInternal::CBlendedEnvironmentTheme&);
}