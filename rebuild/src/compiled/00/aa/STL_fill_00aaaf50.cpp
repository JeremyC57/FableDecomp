// _STL::fill(C3DClothQuad* first, C3DClothQuad* last, const C3DClothQuad& val) -- assign one 16-byte cloth quad (four vertex and four texture-coordinate indices) to every element of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAAF50, 53 bytes (+ int3 padding), `ret 4`: an STLport algorithm
//   instantiation compiled /Gr (`__fastcall`: first in ecx, last in edx, the value reference on the stack).
// Identity: BSim (similarity 1.0) names the Ego_r instantiation `??$fill@PAVC3DClothQuad@@V1@@std@@YIX...`; retail is
//   STLport, so the template lives in namespace _STL. C3DClothQuad is 16 bytes: Ego_r.pdb's constructor
//   `??0C3DClothQuad@@QAE@GGGGGGGG@Z` takes eight indices (VertexIndex[4] +0, TexCoordIndex[4] +8).
// Shape: STLport `for (; first != last; ++first) *first = val;` -- the implicit assignment of the 16-byte POD is a
//   block copy through ebx (three dwords) and esi (the last), the esi/edi pair re-seeded every iteration.
class C3DClothQuad {
public:
    unsigned short VertexIndex[4];                 // +0x00
    unsigned short TexCoordIndex[4];               // +0x08
};

namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(C3DClothQuad* __first, C3DClothQuad* __last, const C3DClothQuad& __val);
}