// std::__copy(ParticleAndEndPoints const* first, ParticleAndEndPoints const* last, ParticleAndEndPoints* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBUParticleAndEndPoints@@PAU1@H@std@@YIPAUParticleAndEndPoints@@PBU1@0PAU1@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00769B90, 97 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0076b1e0 source (stdvector_operator_0076b1e0.cpp) names ParticleAndEndPoints; binder ??$__copy@PBUParticleAndEndPoints@@PAU1@H@std@@YIPAUParticleAndEndPoints@@PBU1@0PAU1@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over ParticleAndEndPoints.
#include "stlport/fable_containers.h"
#include "engine/ParticleAndEndPoints.h"
template ParticleAndEndPoints* std::__copy<ParticleAndEndPoints const*,ParticleAndEndPoints*,int >(ParticleAndEndPoints const*, ParticleAndEndPoints const*, ParticleAndEndPoints*, std::random_access_iterator_tag const&, int*);
