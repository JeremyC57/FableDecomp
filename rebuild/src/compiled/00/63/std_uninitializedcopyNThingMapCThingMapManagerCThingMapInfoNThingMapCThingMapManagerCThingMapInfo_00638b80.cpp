// std::__uninitialized_copy(NThingMap::CThingMapManager::CThingMapInfo* first, NThingMap::CThingMapManager::CThingMapInfo* last, NThingMap::CThingMapManager::CThingMapInfo* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCThingMapInfo@CThingMapManager@NThingMap@@PAV123@@std@@YIPAVCThingMapInfo@CThingMapManager@NThingMap@@PAV123@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00638B80, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00639600 std::vector<NThingMap::CThingMapManager::CThingMapInfo, std::allocator<NThingMap::CThingMapManager::CThingMapInfo>>::_M_fill_insert; caller 00639600 std::vector<NThingMap::CThingMapManager::CThingMapInfo, std::allocator<NThingMap::CThingMapManager::CThingMapInfo>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCThingMapInfo@CThingMapManager@NThingMap@@PAV123@@std@@YIPAVCThingMapInfo@CThingMapManager@NThingMap@@PAV123@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NThingMap::CThingMapManager::CThingMapInfo.
#include "stlport/fable_containers.h"
namespace NThingMap {
class CThingMapManager {
public:
class CThingMapInfo {
public:                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
};
}
template NThingMap::CThingMapManager::CThingMapInfo* std::__uninitialized_copy<NThingMap::CThingMapManager::CThingMapInfo*,NThingMap::CThingMapManager::CThingMapInfo* >(NThingMap::CThingMapManager::CThingMapInfo*, NThingMap::CThingMapManager::CThingMapInfo*, NThingMap::CThingMapManager::CThingMapInfo*, std::__false_type const&);
