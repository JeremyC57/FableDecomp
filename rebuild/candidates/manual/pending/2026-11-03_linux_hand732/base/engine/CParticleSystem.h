#ifndef FABLE_ENGINE_CPARTICLESYSTEM_H
#define FABLE_ENGINE_CPARTICLESYSTEM_H
#include "engine/CBaseClass.h"
#include "engine/CParticleEmitter.h"
class CIEngine;
namespace NParticleEngine {
// Layout: native ctor00af4e10 and original TPI complete fields; size0x70.
class CParticleSystem:public CBaseClass {
public:CParticleSystem();virtual ~CParticleSystem();
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 void SetParentEmitter(const CParticleEmitter& p){PParentEmitter=&p;}
protected:
 const CParticleEmitter* PParentEmitter;CIEngine* PEngine;
 unsigned char Components[16],Particles[4],Decals[4],UpdateParameters[36];
 long NoActiveParticles,NoActiveDecals;
 bool Initialised:1,Enabled:1,Dead:1,NormalisedTimeEnabled:1,UseAllAttractors:1,Visible:1,StayWithEmitter:1,ScaleParticles:1;
 float GlobalAlpha,FadeAlpha,NormalisedTime;
 unsigned char Scale[12];CCharString Name;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleSystem)==0x70);
#endif
