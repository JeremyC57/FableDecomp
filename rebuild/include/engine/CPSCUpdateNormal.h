#ifndef FABLE_ENGINE_CPSCUPDATENORMAL_H
#define FABLE_ENGINE_CPSCUPDATENORMAL_H
#include "engine/CPSCBase.h"
#include "engine/CIntelligentPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NParticleEngine {
class CParticleEmitter;
class CPSCUpdateNormal:public CPSCBase {
public:
 virtual void Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,bool&);
protected:
 float GetCurrentFadeValue()const;
    unsigned long FadeInEndInteger : 10;           // +0x0c
    unsigned long FadeOutBeginInteger : 10;
    unsigned long UseParticleLifeSecs : 1;
    unsigned long UseRandomRotationAxis : 1;
    unsigned long UseRandomInitialRotation : 1;
    unsigned long UseAccelerationParam : 1;
    unsigned long StayWithEmitter : 1;
    unsigned long UseSystemLifeSecs : 1;
    unsigned long SystemAlphaFadeEnable : 1;
    unsigned long EmissionFadeEnable : 1;
    unsigned long ParticleCollideWithGround : 1;
    unsigned long ParticleDieOnCollision : 1;
    unsigned long UseAllAttractors : 1;
    unsigned long RandomisePosEnable : 1;
    bool RandomisePosDistVaryEnable : 1;           // +0x10
    bool SetOrientationFromDirection : 1;
    bool SetOrientationFromGame : 1;
    bool ParticleCreateDecal : 1;
    bool ParticleCreateDecalEmitter : 1;
    bool ParticleCollideWithAnything : 1;
    CCharString DecalEmitterName;                  // +0x14
    float SystemLifeSecs;                          // +0x18
    float ParticleLifeSecs;                        // +0x1c
    float WindFactor;                              // +0x20
    float GravityFactor;                           // +0x24
    float AirResistance;                           // +0x28
    float ParticleAccelerationScale;               // +0x2c
    float InitialRotationX;                        // +0x30
    float InitialRotationY;                        // +0x34
    float InitialRotationZ;                        // +0x38
    float RotationMinAngleSpeed;                   // +0x3c
    float RotationMaxAngleSpeed;                   // +0x40
    float ParticleBounce;                          // +0x44
    float SystemAlphaFadeMinimum;                  // +0x48
    float RandomisePosDistVaryScale;               // +0x4c
    float EmissionFadeMinimum;                     // +0x50
    C3DVector ParticleSystemOffset;                // +0x54
    C3DVector ParticleAcceleration;                // +0x60
    C3DVector RotationAxis;                        // +0x6c
    C3DVector RandomisePosSpeed;                   // +0x78
    C3DVector RandomisePosScale;                   // +0x84
    unsigned long AccelerationParam;               // +0x90
    unsigned long OrientFromGameParam;             // +0x94
    long LifeTimer;                                // +0x98
    std::list<CIntelligentPointer<CParticleEmitter> > DecalEmitterList;   // +0x9c
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCUpdateNormal)==0xa0);
#endif
