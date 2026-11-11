// std::__copy(CEngineLightingManager::CThemeChannel* first, CEngineLightingManager::CThemeChannel* last, CEngineLightingManager::CThemeChannel* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAUCThemeChannel@CEngineLightingManager@@PAU12@H@std@@YIPAUCThemeChannel@CEngineLightingManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B490F0, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00b4a4c0 std::vector<CEngineLightingManager::CThemeChannel, std::allocator<CEngineLightingManager::CThemeChannel>>::resize; binder ??$__copy@PAUCThemeChannel@CEngineLightingManager@@PAU12@H@std@@YIPAUCThemeChannel@CEngineLightingManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLightingManager::CThemeChannel.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#include "stlport/fable_containers.h"
#include "engine/CEngineLightingManager.h"
template CEngineLightingManager::CThemeChannel* std::__copy<CEngineLightingManager::CThemeChannel*,CEngineLightingManager::CThemeChannel*,int >(CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*, std::random_access_iterator_tag const&, int*);
