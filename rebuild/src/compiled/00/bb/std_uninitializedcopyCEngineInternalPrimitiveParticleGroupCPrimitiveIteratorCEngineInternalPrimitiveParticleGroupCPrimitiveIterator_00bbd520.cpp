// std::__uninitialized_copy(CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* first, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* last, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@PAV12@@std@@YIPAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBD520, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bbd950 std::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator, std::allocator<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>>::_M_fill_insert; caller 00bbd950 std::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator, std::allocator<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@PAV12@@std@@YIPAVCPrimitiveIterator@CEngineInternalPrimitiveParticleGroup@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator.
#include "stlport/fable_containers.h"
class CEngineInternalPrimitiveParticleGroup {
public:
class CPrimitiveIterator {
public:                 // std::pair<bool, long>, 8 bytes
    bool first;                                 // +0x00
    long second;                                // +0x04
    CPrimitiveIterator(const CPrimitiveIterator& o) : first(o.first), second(o.second) { }
};
};
template CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* std::__uninitialized_copy<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*,CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator* >(CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator*, std::__false_type const&);
