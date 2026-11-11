// std::uninitialized_fill_n(NThingMap::CThingMapManager::CThingMapInfo* first, unsigned int n, NThingMap::CThingMapManager::CThingMapInfo const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCThingMapInfo@CThingMapManager@NThingMap@@IV123@@std@@YIPAVCThingMapInfo@CThingMapManager@NThingMap@@PAV123@IABV123@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00638BE0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00639600 std::vector<NThingMap::CThingMapManager::CThingMapInfo, std::allocator<NThingMap::CThingMapManager::CThingMapInfo>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCThingMapInfo@CThingMapManager@NThingMap@@IV123@@std@@YIPAVCThingMapInfo@CThingMapManager@NThingMap@@PAV123@IABV123@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NThingMap::CThingMapManager::CThingMapInfo.
#include "stlport/fable_containers.h"
namespace NThingMap {
class CThingMapManager {
public:
class CThingMapInfo {
public:
    long first;
    long second;
};
};
}
template NThingMap::CThingMapManager::CThingMapInfo* std::uninitialized_fill_n<NThingMap::CThingMapManager::CThingMapInfo*,unsigned int,NThingMap::CThingMapManager::CThingMapInfo >(NThingMap::CThingMapManager::CThingMapInfo*, unsigned int, NThingMap::CThingMapManager::CThingMapInfo const&);
