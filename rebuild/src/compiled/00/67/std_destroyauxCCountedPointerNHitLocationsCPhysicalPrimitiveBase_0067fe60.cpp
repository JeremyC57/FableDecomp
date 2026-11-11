// std::__destroy_aux(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>* first, CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@@std@@YIXPAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067FE60, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0067ff30 source (stdvector_Minsertoverflow_0067ff30.cpp) names CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>; caller 01229210 source (global_E2_01229210.cpp) names CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@@std@@YIXPAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NHitLocations_CPhysicalPrimitiveBase.h"
template void std::__destroy_aux<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>* >(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>*, CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>*, std::__false_type const&);
