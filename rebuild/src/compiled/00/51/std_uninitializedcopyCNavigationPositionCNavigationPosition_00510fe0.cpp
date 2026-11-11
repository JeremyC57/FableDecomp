// std::__uninitialized_copy(CNavigationPosition* first, CNavigationPosition* last, CNavigationPosition* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCNavigationPosition@@PAV1@@std@@YIPAVCNavigationPosition@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00510FE0, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00517ba0 std::vector<CNavigationPosition>::_M_insert_overflow; binder ??$__uninitialized_copy@PAVCNavigationPosition@@PAV1@@std@@YIPAVCNavigationPosition@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CNavigationPosition.
#include "stlport/fable_containers.h"
#include "engine/CNavigationPosition.h"
template CNavigationPosition* std::__uninitialized_copy<CNavigationPosition*,CNavigationPosition* >(CNavigationPosition*, CNavigationPosition*, CNavigationPosition*, std::__false_type const&);
