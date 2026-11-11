// std::fill(CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* first, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* last, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@V12@@std@@YIXPAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBD380, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00bbd950 std::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator, std::allocator<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>>::_M_fill_insert; caller 00bbd950 std::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator, std::allocator<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>>::_M_fill_insert; binder ??$fill@PAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@V12@@std@@YIXPAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator.
#include "stlport/fable_containers.h"
class CEngineInternalPrimitiveParticleGroup {
public:
class CPrimitiveIterator {
public:
    unsigned char _body[0x8];
};
};
template void std::fill<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*,CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator >(CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator const&);
