// std::__uninitialized_copy(NThingMap::CThingMapCell const* first, NThingMap::CThingMapCell const* last, NThingMap::CThingMapCell* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVCThingMapCell@NThingMap@@PAV12@@std@@YIPAVCThingMapCell@NThingMap@@PBV12@0PAV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085D820, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0085dfd0 source (stdvectorCThingMapCell_operator_0085dfd0.cpp) names NThingMap::CThingMapCell; binder ??$__uninitialized_copy@PBVCThingMapCell@NThingMap@@PAV12@@std@@YIPAVCThingMapCell@NThingMap@@PBV12@0PAV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NThingMap::CThingMapCell.
#include "stlport/fable_containers.h"
namespace NThingMap {
class CThingMapCell {
public: long m_0;
};
}
template NThingMap::CThingMapCell* std::__uninitialized_copy<NThingMap::CThingMapCell const*,NThingMap::CThingMapCell* >(NThingMap::CThingMapCell const*, NThingMap::CThingMapCell const*, NThingMap::CThingMapCell*, std::__false_type const&);
