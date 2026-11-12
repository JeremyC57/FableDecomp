#ifndef FABLE_ENGINE_CPARTICLEEMITTERLIST_H
#define FABLE_ENGINE_CPARTICLEEMITTERLIST_H
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterList {
#ifdef FABLE_RETAIL_CPARTICLEEMITTERLIST_INSERT
public:
 // Opt-in (chunk Q28, row 0x00AEB790): Ego_r.pdb / FableWin.pdb
 // ?Insert@CParticleEmitterList@NParticleEngine@@QAE_NAAV?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@@Z (0x00AEB060).
 bool Insert(CCountedPointer<CParticleEmitter>& emitter);              // 0x00AEB060
#endif
private:std::list<CCountedPointer<CParticleEmitter> > Active;long MaxActiveEmitters;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitterList)==8);
#endif
