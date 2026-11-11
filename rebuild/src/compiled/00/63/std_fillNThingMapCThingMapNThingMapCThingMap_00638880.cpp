// std::fill(NThingMap::CThingMap** first, NThingMap::CThingMap** last, NThingMap::CThingMap* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCThingMap@NThingMap@@PAV12@@std@@YIXPAPAVCThingMap@NThingMap@@0ABQAV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00638880, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 006392b0 source (stdvector_Mfillinsert_006392b0.cpp) names NThingMap::CThingMap; binder ??$fill@PAPAVCThingMap@NThingMap@@PAV12@@std@@YIXPAPAVCThingMap@NThingMap@@0ABQAV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NThingMap::CThingMap*.
#include "stlport/fable_containers.h"
namespace NThingMap {
class CThingMap;
}
template void std::fill<NThingMap::CThingMap**,NThingMap::CThingMap* >(NThingMap::CThingMap**, NThingMap::CThingMap**, NThingMap::CThingMap* const&);
