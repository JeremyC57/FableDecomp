// std::uninitialized_fill_n(_D3DVERTEXELEMENT9* first, unsigned int n, _D3DVERTEXELEMENT9 const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU_D3DVERTEXELEMENT9@@IU1@@std@@YIPAU_D3DVERTEXELEMENT9@@PAU1@IABU1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB3F30, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ab4260 std::vector<_D3DVERTEXELEMENT9, std::allocator<_D3DVERTEXELEMENT9>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAU_D3DVERTEXELEMENT9@@IU1@@std@@YIPAU_D3DVERTEXELEMENT9@@PAU1@IABU1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over _D3DVERTEXELEMENT9.
#include "stlport/fable_containers.h"
struct _D3DVERTEXELEMENT9 {
    long first;
    long second;
};
template _D3DVERTEXELEMENT9* std::uninitialized_fill_n<_D3DVERTEXELEMENT9*,unsigned int,_D3DVERTEXELEMENT9 >(_D3DVERTEXELEMENT9*, unsigned int, _D3DVERTEXELEMENT9 const&);
