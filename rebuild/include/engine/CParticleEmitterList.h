#ifndef FABLE_ENGINE_CPARTICLEEMITTERLIST_H
#define FABLE_ENGINE_CPARTICLEEMITTERLIST_H
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterList {
private:std::list<CCountedPointer<CParticleEmitter> > Active;long MaxActiveEmitters;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitterList)==8);
#endif
