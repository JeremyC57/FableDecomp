// std::__copy(CNavigationPosition const* first, CNavigationPosition const* last, CNavigationPosition* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBVCNavigationPosition@@PAV1@H@std@@YIPAVCNavigationPosition@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7D290, 74 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a80860 std::vector<CNavigationPosition, std::allocator<CNavigationPosition>>::operator=; caller 00a80860 std::vector<CNavigationPosition, std::allocator<CNavigationPosition>>::operator=; binder ??$__copy@PBVCNavigationPosition@@PAV1@H@std@@YIPAVCNavigationPosition@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CNavigationPosition.
#include "stlport/fable_containers.h"
#include "engine/CNavigationPosition.h"
template CNavigationPosition* std::__copy<CNavigationPosition const*,CNavigationPosition*,int >(CNavigationPosition const*, CNavigationPosition const*, CNavigationPosition*, std::random_access_iterator_tag const&, int*);
