// std::__uninitialized_fill_n(CCountedPointer<CParticleSimulation>* first, unsigned int n, CCountedPointer<CParticleSimulation> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCParticleSimulation@@@@IV1@@std@@YIPAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B82D00, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00b83970 source (stdvector_Minsertoverflow_00b83970.cpp) names CCountedPointer<CParticleSimulation>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCParticleSimulation@@@@IV1@@std@@YIPAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CParticleSimulation>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CParticleSimulation.h"
template CCountedPointer<CParticleSimulation>* std::__uninitialized_fill_n<CCountedPointer<CParticleSimulation>*,unsigned int,CCountedPointer<CParticleSimulation> >(CCountedPointer<CParticleSimulation>*, unsigned int, CCountedPointer<CParticleSimulation> const&, std::__false_type const&);
