// std::__destroy_aux(CCountedPointer<NParticleEngine::CParticleSystem>* first, CCountedPointer<NParticleEngine::CParticleSystem>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCParticleSystem@NParticleEngine@@@@@std@@YIXPAV?$CCountedPointer@VCParticleSystem@NParticleEngine@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF0D80, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity (agentNF20, 2026-11-11; reverts the FAM02 CGuiVarTransferBase rename): every retail caller is a
//   vector<CCountedPointer<NParticleEngine::CParticleSystem> > member -- operator= 0x00AF0E10, reserve 0x00AF0EF0 (FableWin.pdb
//   ?reserve@?$vector@V?$CCountedPointer@VCParticleSystem@NParticleEngine@@@@...) and the inline vector dtor in
//   NParticleEngine::CParticleEmitter::~CParticleEmitter 0x00AF10E0 (Ego_r CParticleEmitter::ParticleSystems) -- except the
//   COMDAT-folded vector<CCountedPointer<CGuiVarTransferBase> >::_M_insert_overflow 0x00AF0FA0, whose own (folded) instance
//   is the CGuiVarTransferBase one. The address carries the particle-system instance name.
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<NParticleEngine::CParticleSystem>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CParticleSystem; }
template void std::__destroy_aux<CCountedPointer<NParticleEngine::CParticleSystem>* >(CCountedPointer<NParticleEngine::CParticleSystem>*, CCountedPointer<NParticleEngine::CParticleSystem>*, std::__false_type const&);
