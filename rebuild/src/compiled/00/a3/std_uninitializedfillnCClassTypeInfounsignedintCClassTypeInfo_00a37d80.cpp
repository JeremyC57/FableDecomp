// std::uninitialized_fill_n(CClassTypeInfo* first, unsigned int n, CClassTypeInfo const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCClassTypeInfo@@IV1@@std@@YIPAVCClassTypeInfo@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A37D80, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a37f40 std::vector<CClassTypeInfo, std::allocator<CClassTypeInfo>>::_M_fill_insert; callee 00a37b50 CClassTypeInfo::CClassTypeInfo; binder ??$uninitialized_fill_n@PAVCClassTypeInfo@@IV1@@std@@YIPAVCClassTypeInfo@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CClassTypeInfo.
#include "stlport/fable_containers.h"
#include "engine/CClassTypeInfo.h"
template CClassTypeInfo* std::uninitialized_fill_n<CClassTypeInfo*,unsigned int,CClassTypeInfo >(CClassTypeInfo*, unsigned int, CClassTypeInfo const&);
