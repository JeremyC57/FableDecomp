// std::__uninitialized_fill_n(ParticleAndEndPoints* first, unsigned int n, ParticleAndEndPoints const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAUParticleAndEndPoints@@IU1@@std@@YIPAUParticleAndEndPoints@@PAU1@IABU1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00769810, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0076b850 source (stdvector_Minsertoverflow_0076b850.cpp) names ParticleAndEndPoints; binder ??$__uninitialized_fill_n@PAUParticleAndEndPoints@@IU1@@std@@YIPAUParticleAndEndPoints@@PAU1@IABU1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over ParticleAndEndPoints.
#include "stlport/fable_containers.h"
#include "engine/ParticleAndEndPoints.h"
template ParticleAndEndPoints* std::__uninitialized_fill_n<ParticleAndEndPoints*,unsigned int,ParticleAndEndPoints >(ParticleAndEndPoints*, unsigned int, ParticleAndEndPoints const&, std::__false_type const&);
