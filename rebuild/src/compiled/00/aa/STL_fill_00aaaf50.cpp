// std::fill(CPlane* first, CPlane* last, const CPlane& val) -- assign one 16-byte plane (normal and distance) to every element of [first, last)
// Leaf: ??$fill@PAVCPlane@@V1@@std@@YIXPAVCPlane@@0ABV1@@Z
// Name fix (NF382): was the fill<C3DClothQuad*> instance (FableWin: called from vector<C3DClothQuad>::_Insert_n in
//   lib_particle_simulation.obj, not from this address's caller). Both call sites 0x00AAB608 / 0x00AAB67D are in the landed
//   vector<CPlane>::_M_fill_insert 0x00AAB570 (its caller C3DMeshFileConvexVolumeChunk::ReadFromFile 0x00AAAB10 fills the Planes
//   vector), and FableWin vector<CPlane>::_Insert_n calls fill<CPlane*> (lib_physics_mesh_object_2.obj). Same bytes.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAAF50, 53 bytes (+ int3 padding), `ret 4`: an STLport algorithm
//   instantiation compiled /Gr (`__fastcall`: first in ecx, last in edx, the value reference on the stack).
// Identity: STLport fill; retail is STLport, so the template lives in namespace std. CPlane is 16 bytes (Ego_r.pdb and
//   engine/CPlane.h: Normal C3DVector +0, Distance +0xC), the record this body copies.
// Shape: STLport `for (; first != last; ++first) *first = val;` -- the implicit assignment of the 16-byte record is a
//   block copy through ebx (three dwords) and esi (the last), the esi/edi pair re-seeded every iteration.
#include "engine/CPlane.h"

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill(CPlane* __first, CPlane* __last, const CPlane& __val);
}