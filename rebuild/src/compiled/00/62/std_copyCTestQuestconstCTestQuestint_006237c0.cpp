// std::__copy(CTestQuest const* first, CTestQuest const* last, CTestQuest* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBVCTestQuest@@PAV1@H@std@@YIPAVCTestQuest@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006237C0, 137 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00624a30 std::vector<CTestQuest, std::allocator<CTestQuest>>::operator=; caller 00624a30 std::vector<CTestQuest, std::allocator<CTestQuest>>::operator=; binder ??$__copy@PBVCTestQuest@@PAV1@H@std@@YIPAVCTestQuest@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTestQuest.
#include "stlport/fable_containers.h"
#include "engine/CTestQuest.h"
template CTestQuest* std::__copy<CTestQuest const*,CTestQuest*,int >(CTestQuest const*, CTestQuest const*, CTestQuest*, std::random_access_iterator_tag const&, int*);
