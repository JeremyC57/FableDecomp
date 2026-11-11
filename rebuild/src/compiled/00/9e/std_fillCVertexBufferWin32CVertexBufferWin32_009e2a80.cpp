// std::fill(CVertexBufferWin32** first, CVertexBufferWin32** last, CVertexBufferWin32* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCVertexBufferWin32@@PAV1@@std@@YIXPAPAVCVertexBufferWin32@@0ABQAV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2A80, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009e3430 std::vector<CVertexBufferWin32 *, std::allocator<CVertexBufferWin32 *>>::_M_fill_insert; caller 009e3430 std::vector<CVertexBufferWin32 *, std::allocator<CVertexBufferWin32 *>>::_M_fill_insert; binder ??$fill@PAPAVCVertexBufferWin32@@PAV1@@std@@YIXPAPAVCVertexBufferWin32@@0ABQAV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CVertexBufferWin32*.
#include "stlport/fable_containers.h"
class CVertexBufferWin32;
template void std::fill<CVertexBufferWin32**,CVertexBufferWin32* >(CVertexBufferWin32**, CVertexBufferWin32**, CVertexBufferWin32* const&);
