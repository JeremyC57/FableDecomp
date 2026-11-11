// std::fill(NThingMap::CThingMapManager::CThingMapInfo* first, NThingMap::CThingMapManager::CThingMapInfo* last, NThingMap::CThingMapManager::CThingMapInfo const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCThingMapInfo@CThingMapManager@NThingMap@@V123@@std@@YIXPAVCThingMapInfo@CThingMapManager@NThingMap@@0ABV123@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006387C0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00639600 std::vector<NThingMap::CThingMapManager::CThingMapInfo, std::allocator<NThingMap::CThingMapManager::CThingMapInfo>>::_M_fill_insert; caller 00639600 std::vector<NThingMap::CThingMapManager::CThingMapInfo, std::allocator<NThingMap::CThingMapManager::CThingMapInfo>>::_M_fill_insert; binder ??$fill@PAVCThingMapInfo@CThingMapManager@NThingMap@@V123@@std@@YIXPAVCThingMapInfo@CThingMapManager@NThingMap@@0ABV123@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NThingMap::CThingMapManager::CThingMapInfo.
#include "stlport/fable_containers.h"
namespace NThingMap {
class CThingMapManager {
public:
class CThingMapInfo {
public: unsigned char _body[0x8];
};
};
}
template void std::fill<NThingMap::CThingMapManager::CThingMapInfo*,NThingMap::CThingMapManager::CThingMapInfo >(NThingMap::CThingMapManager::CThingMapInfo*, NThingMap::CThingMapManager::CThingMapInfo*, NThingMap::CThingMapManager::CThingMapInfo const&);
