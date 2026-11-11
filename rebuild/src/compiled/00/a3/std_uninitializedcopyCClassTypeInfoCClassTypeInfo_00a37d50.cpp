// std::__uninitialized_copy(CClassTypeInfo* first, CClassTypeInfo* last, CClassTypeInfo* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCClassTypeInfo@@PAV1@@std@@YIPAVCClassTypeInfo@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A37D50, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a37e20 std::vector<CClassTypeInfo, std::allocator<CClassTypeInfo>>::_M_insert_overflow; caller 00a37f40 std::vector<CClassTypeInfo, std::allocator<CClassTypeInfo>>::_M_fill_insert; caller 00a37f40 std::vector<CClassTypeInfo, std::allocator<CClassTypeInfo>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCClassTypeInfo@@PAV1@@std@@YIPAVCClassTypeInfo@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CClassTypeInfo.
#include "stlport/fable_containers.h"
#include "engine/CClassTypeInfo.h"
template CClassTypeInfo* std::__uninitialized_copy<CClassTypeInfo*,CClassTypeInfo* >(CClassTypeInfo*, CClassTypeInfo*, CClassTypeInfo*, std::__false_type const&);
