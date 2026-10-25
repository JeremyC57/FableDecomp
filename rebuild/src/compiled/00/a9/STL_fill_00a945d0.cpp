// std::fill(C3DTriangle2* first, C3DTriangle2* last, const C3DTriangle2& val) -- assign one 6-byte triangle (three vertex indices) to every element of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A945D0, 44 bytes (+ int3 padding), `ret 4`: an STLport algorithm
//   instantiation compiled /Gr (`__fastcall`: first in ecx, last in edx, the value reference on the stack).
// Identity: BSim (similarity 1.0) names the Ego_r instantiation `??$fill@PAVC3DTriangle2@@V1@@std@@YIX...`; retail is
//   STLport, so the template lives in namespace std. C3DTriangle2 is 6 bytes (Ego_r.pdb: ushort VertexIndex[3]).
// Shape: STLport `for (; first != last; ++first) *first = val;` -- the implicit assignment of the 6-byte POD is a block
//   copy (dword through ebx, word through si, the esi/edi source/destination pair re-seeded every iteration).
class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];                 // +0x00
};

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(C3DTriangle2* __first, C3DTriangle2* __last, const C3DTriangle2& __val);
}