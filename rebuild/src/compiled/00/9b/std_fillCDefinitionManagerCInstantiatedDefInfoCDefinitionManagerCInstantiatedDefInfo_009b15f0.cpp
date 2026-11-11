// std::fill(CDefinitionManager::CInstantiatedDefInfo** first, CDefinitionManager::CInstantiatedDefInfo** last, CDefinitionManager::CInstantiatedDefInfo* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCInstantiatedDefInfo@CDefinitionManager@@PAV12@@std@@YIXPAPAVCInstantiatedDefInfo@CDefinitionManager@@0ABQAV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B15F0, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009b4ff0 std::vector<CDefinitionManager::CInstantiatedDefInfo *, std::allocator<CDefinitionManager::CInstantiatedDefInfo *>>::_M_fill_insert; caller 009b4ff0 std::vector<CDefinitionManager::CInstantiatedDefInfo *, std::allocator<CDefinitionManager::CInstantiatedDefInfo *>>::_M_fill_insert; binder ??$fill@PAPAVCInstantiatedDefInfo@CDefinitionManager@@PAV12@@std@@YIXPAPAVCInstantiatedDefInfo@CDefinitionManager@@0ABQAV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CDefinitionManager::CInstantiatedDefInfo*.
#include "stlport/fable_containers.h"
class CDefinitionManager {
public:
class CInstantiatedDefInfo;
};
template void std::fill<CDefinitionManager::CInstantiatedDefInfo**,CDefinitionManager::CInstantiatedDefInfo* >(CDefinitionManager::CInstantiatedDefInfo**, CDefinitionManager::CInstantiatedDefInfo**, CDefinitionManager::CInstantiatedDefInfo* const&);
