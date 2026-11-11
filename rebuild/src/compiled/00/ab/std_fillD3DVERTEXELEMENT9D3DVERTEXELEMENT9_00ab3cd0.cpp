// std::fill(_D3DVERTEXELEMENT9* first, _D3DVERTEXELEMENT9* last, _D3DVERTEXELEMENT9 const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAU_D3DVERTEXELEMENT9@@U1@@std@@YIXPAU_D3DVERTEXELEMENT9@@0ABU1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB3CD0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ab4260 std::vector<_D3DVERTEXELEMENT9, std::allocator<_D3DVERTEXELEMENT9>>::_M_fill_insert; caller 00ab4260 std::vector<_D3DVERTEXELEMENT9, std::allocator<_D3DVERTEXELEMENT9>>::_M_fill_insert; binder ??$fill@PAU_D3DVERTEXELEMENT9@@U1@@std@@YIXPAU_D3DVERTEXELEMENT9@@0ABU1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over _D3DVERTEXELEMENT9.
#include "stlport/fable_containers.h"
struct _D3DVERTEXELEMENT9 {
    unsigned char _body[0x8];
};
template void std::fill<_D3DVERTEXELEMENT9*,_D3DVERTEXELEMENT9 >(_D3DVERTEXELEMENT9*, _D3DVERTEXELEMENT9*, _D3DVERTEXELEMENT9 const&);
