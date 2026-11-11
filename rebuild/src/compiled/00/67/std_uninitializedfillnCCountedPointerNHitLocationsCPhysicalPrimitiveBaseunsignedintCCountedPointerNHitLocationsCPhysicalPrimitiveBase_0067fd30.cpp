// std::__uninitialized_fill_n(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>* first, unsigned int n, CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@IV1@@std@@YIPAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067FD30, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0067ff30 source (stdvector_Minsertoverflow_0067ff30.cpp) names CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@IV1@@std@@YIPAV?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/NHitLocations_CPhysicalPrimitiveBase.h"
template CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>* std::__uninitialized_fill_n<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>*,unsigned int,CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> >(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>*, unsigned int, CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> const&, std::__false_type const&);
