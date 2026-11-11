// std::__uninitialized_copy(_D3DVERTEXELEMENT9* first, _D3DVERTEXELEMENT9* last, _D3DVERTEXELEMENT9* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU_D3DVERTEXELEMENT9@@PAU1@@std@@YIPAU_D3DVERTEXELEMENT9@@PAU1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB3F00, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ab4260 std::vector<_D3DVERTEXELEMENT9, std::allocator<_D3DVERTEXELEMENT9>>::_M_fill_insert; caller 00ab4260 std::vector<_D3DVERTEXELEMENT9, std::allocator<_D3DVERTEXELEMENT9>>::_M_fill_insert; binder ??$__uninitialized_copy@PAU_D3DVERTEXELEMENT9@@PAU1@@std@@YIPAU_D3DVERTEXELEMENT9@@PAU1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over _D3DVERTEXELEMENT9.
#include "stlport/fable_containers.h"
struct _D3DVERTEXELEMENT9 {                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
template _D3DVERTEXELEMENT9* std::__uninitialized_copy<_D3DVERTEXELEMENT9*,_D3DVERTEXELEMENT9* >(_D3DVERTEXELEMENT9*, _D3DVERTEXELEMENT9*, _D3DVERTEXELEMENT9*, std::__false_type const&);
