// std::__uninitialized_copy(CNavigationPosition const* first, CNavigationPosition const* last, CNavigationPosition* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCNavigationPosition@@PAV1@@std@@YIPAVCNavigationPosition@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7E990, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a80860 std::vector<CNavigationPosition, std::allocator<CNavigationPosition>>::operator=; binder ??$__uninitialized_copy@PBVCNavigationPosition@@PAV1@@std@@YIPAVCNavigationPosition@@PBV1@0PAV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CNavigationPosition.
#include "stlport/fable_containers.h"
#include "engine/CNavigationPosition.h"
template CNavigationPosition* std::__uninitialized_copy<CNavigationPosition const*,CNavigationPosition* >(CNavigationPosition const*, CNavigationPosition const*, CNavigationPosition*, std::__false_type const&);
