// std::fill(CEngineLightingManager::CThemeChannel* first, CEngineLightingManager::CThemeChannel* last, CEngineLightingManager::CThemeChannel const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAUCThemeChannel@CEngineLightingManager@@U12@@std@@YIXPAUCThemeChannel@CEngineLightingManager@@0ABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B48EF0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b4a2c0 std::vector<CEngineLightingManager::CThemeChannel, std::allocator<CEngineLightingManager::CThemeChannel>>::_M_fill_insert; caller 00b4a2c0 std::vector<CEngineLightingManager::CThemeChannel, std::allocator<CEngineLightingManager::CThemeChannel>>::_M_fill_insert; binder ??$fill@PAUCThemeChannel@CEngineLightingManager@@U12@@std@@YIXPAUCThemeChannel@CEngineLightingManager@@0ABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLightingManager::CThemeChannel.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#include "stlport/fable_containers.h"
#include "engine/CEngineLightingManager.h"
template void std::fill<CEngineLightingManager::CThemeChannel*,CEngineLightingManager::CThemeChannel >(CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel const&);
