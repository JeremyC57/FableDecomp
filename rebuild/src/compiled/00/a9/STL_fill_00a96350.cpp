// std::fill(CCountedPointer<C3DClothPrimitive>* first, CCountedPointer<C3DClothPrimitive>* last, const CCountedPointer<C3DClothPrimitive>& value) -- assign one counted pointer to every slot of [first, last), releasing each slot's old reference
// Leaf: ??$fill@PAV?$CCountedPointer@VC3DClothPrimitive@@@@V1@@std@@YIXPAV?$CCountedPointer@VC3DClothPrimitive@@@@0ABV1@@Z
// Name fix (NF382): was the fill<CCountedPointer<C3DGroup2>*> instance. The only call site 0x00A97F21 is in the landed
//   vector<CCountedPointer<C3DClothPrimitive> >::_M_fill_insert 0x00A97E40 (reached from C3DPrimitive2::LoadBinary 0x00A92900 and
//   that vector's resize 0x00A98540), and FableWin vector<CCountedPointer<C3DClothPrimitive> >::_Insert_n calls
//   fill<CCountedPointer<C3DClothPrimitive>*> (lib_3d_primitive_2.obj). Same bytes (the pointee type never reaches the code).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96350, 94 bytes, `__fastcall` (first ecx, last edx, value on the stack),
//   `ret 4`.
// Identity: the only call site 0x00A97F21 is the overlap arm of vector<CCountedPointer<..> >::_M_fill_insert 0x00A97E40
//   (`fill(pos, pos + n, x_copy)` after the x_copy counted copy and the uninitialized_copy/copy_backward calls). The
//   shape is fill, not the catalog's propagated `_Copy_backward<CCountedPointer<C3DGroup2>*>`; the pointee type is
//   is the caller's element type (NF382).
// Callees: the count block's destroy hook (`mov ecx,[info+8] ; call [info+4]`, a __fastcall DeleteData), operator delete
//   0x00BFE9BC.
// Shape: SGI `for (; first != last; ++first) *first = value;` (`last` spilled to the `push ecx` frame word, reloaded after
//   the release call) with CCountedPointer::operator= inline exactly as in the landed counted-pointer models: read
//   other.Info/other.Data first, skip when Info is unchanged, otherwise release the old block (decrement, re-read,
//   destroy + delete at zero), store both words and bump the new count.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

#include "engine/CCountedPointer.h"


class C3DClothPrimitive;

namespace std {

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill<CCountedPointer<C3DClothPrimitive>*, CCountedPointer<C3DClothPrimitive> >(
    CCountedPointer<C3DClothPrimitive>* __first, CCountedPointer<C3DClothPrimitive>* __last, const CCountedPointer<C3DClothPrimitive>& __val);
}