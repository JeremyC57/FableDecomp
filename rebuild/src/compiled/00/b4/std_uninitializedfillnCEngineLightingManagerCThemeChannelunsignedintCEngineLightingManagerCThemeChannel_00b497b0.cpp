// std::uninitialized_fill_n(CEngineLightingManager::CThemeChannel* first, unsigned int n, CEngineLightingManager::CThemeChannel const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAUCThemeChannel@CEngineLightingManager@@IU12@@std@@YIPAUCThemeChannel@CEngineLightingManager@@PAU12@IABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B497B0, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b4a2c0 std::vector<CEngineLightingManager::CThemeChannel, std::allocator<CEngineLightingManager::CThemeChannel>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAUCThemeChannel@CEngineLightingManager@@IU12@@std@@YIPAUCThemeChannel@CEngineLightingManager@@PAU12@IABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLightingManager::CThemeChannel.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#include "stlport/fable_containers.h"
#include "engine/CEngineLightingManager.h"
template CEngineLightingManager::CThemeChannel* std::uninitialized_fill_n<CEngineLightingManager::CThemeChannel*,unsigned int,CEngineLightingManager::CThemeChannel >(CEngineLightingManager::CThemeChannel*, unsigned int, CEngineLightingManager::CThemeChannel const&);
