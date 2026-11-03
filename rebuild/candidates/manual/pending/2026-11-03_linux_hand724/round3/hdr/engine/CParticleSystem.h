#ifndef FABLE_ENGINE_CPARTICLESYSTEM_H
#define FABLE_ENGINE_CPARTICLESYSTEM_H
#include "engine/CBaseClass.h"
#include "engine/CParticleEmitter.h"
class CIEngine;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#endif
namespace NParticleEngine {
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
class CDecal;class CParticle;
#endif
// Layout: native ctor00af4e10 and original TPI complete fields; size0x70.
class CParticleSystem:public CBaseClass {
public:CParticleSystem();virtual ~CParticleSystem();
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 void SetParentEmitter(const CParticleEmitter& p){PParentEmitter=&p;}
protected:
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
 // chunk 724c (FableWin.pdb ?<name>@CParticleSystem@NParticleEngine@@, QAE/QBE): the decal list and the two alphas read by
 // CPSCDecalRenderer::PreparePrimitives 0x00B1F200 (inline), ClearDecal / ScaleParticleDimensions(C2DVector&) out of line.
public:
 std::list<CDecal*>& GetDecalList(){return Decals;}
 float GetGlobalAlpha()const{return GlobalAlpha;}
 float GetFadeAlpha()const{return FadeAlpha;}
 void ClearDecal(std::list<CDecal*>::iterator&);           // 0x00AF5430
 void ScaleParticleDimensions(C2DVector&)const;            // 0x00AF8F00
protected:
 const CParticleEmitter* PParentEmitter;CIEngine* PEngine;
 unsigned char Components[16];std::list<CParticle*> Particles;std::list<CDecal*> Decals;unsigned char UpdateParameters[36];
#else
 const CParticleEmitter* PParentEmitter;CIEngine* PEngine;
 unsigned char Components[16],Particles[4],Decals[4],UpdateParameters[36];
#endif
 long NoActiveParticles,NoActiveDecals;
 bool Initialised:1,Enabled:1,Dead:1,NormalisedTimeEnabled:1,UseAllAttractors:1,Visible:1,StayWithEmitter:1,ScaleParticles:1;
 float GlobalAlpha,FadeAlpha,NormalisedTime;
 unsigned char Scale[12];CCharString Name;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleSystem)==0x70);
#endif
