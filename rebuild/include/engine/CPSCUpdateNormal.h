#if !defined(FABLE_ENGINE_CPSCUPDATENORMAL_H) && !defined(FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS)
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
#ifdef FABLE_RETAIL_CPSCUPDATENORMAL_COLLECT_PARAMS_PASS30
 // Ego_r psc_update_normal.cpp and retail slot 10 (0x00B0F210).
 virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>&,const unsigned long&);
#endif
#ifdef FABLE_RETAIL_CPSCUPDATENORMAL_UPDATE_ADD
 virtual void UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,C3DVector,C3DVector,float);
#endif
 virtual void Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,bool&);
#ifdef FABLE_RETAIL_CPSCUPDATENORMAL_DATABASE_SERIALISE
 // Retail opt-in (Codex pass 24): Ego_r and FableWin give the override in vtable slot 17, 0x00B0D360.
 virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&);
#endif
#ifdef FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE
 // NF158 opt-in (row 0x00B0F990 ??_GCPSCUpdateNormal): the destructor body (Ego_r.pdb ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ,
 // retail 0x00B0CA90: store the vtable, KillAllDecalEmitters, then the members) visible, so the scalar deleting destructor expands it.
 virtual ~CPSCUpdateNormal() { KillAllDecalEmitters(); }
protected:
 void KillAllDecalEmitters();   // 0x00B0C730 (Ego_r + FableWin ?KillAllDecalEmitters@CPSCUpdateNormal@NParticleEngine@@IAEXXZ)
#endif
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
FABLE_STATIC_ASSERT_AT(63, sizeof(NParticleEngine::CPSCUpdateNormal)==0xa0);
#endif
#ifdef FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS
// Add-only per-TU opt-in (NF462, 2026-11-26, ruling r460d on r451a / r459d): the class above without STLport. The typed view's
// std::list<CIntelligentPointer<CParticleEmitter> > DecalEmitterList (stlport/fable_containers.h -> <new>, which adds /DEFAULTLIB:"libcp" to
// the object's .drectve; retail's objects never had it, r450a) keeps its name and offset here as 4 opaque bytes (TLC STLport list); every
// other member is the typed view's, copied in its order. For TUs that only name or allocate the class (no member construction, destruction
// or list access). The typed view's declaration-only opt-ins are not carried; the inline-destructor opt-in destroys the list.
#if defined(FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE) || defined(FABLE_RETAIL_CPSCUPDATENORMAL_UPDATE_ADD) || defined(FABLE_RETAIL_CPSCUPDATENORMAL_DATABASE_SERIALISE)
#error "FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS cannot be combined with the typed view's _DTOR_INLINE / _UPDATE_ADD / _DATABASE_SERIALISE opt-ins"
#endif
#ifndef FABLE_ENGINE_CPSCUPDATENORMAL_H
#define FABLE_ENGINE_CPSCUPDATENORMAL_H
#include <stddef.h>
#include "engine/CPSCBase.h"
#include "fable_string.h"
namespace NParticleEngine {
class CParticleEmitter;
class CPSCUpdateNormal:public CPSCBase {
public:
 virtual void Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,bool&);
#ifdef FABLE_RETAIL_CPSCUPDATENORMAL_ALLOC
 // Opt-in (NF462, ruling r460d; the engine/NParticleEngine_CPSCOrbit.h pattern): the class factory and the members a TU needs to `new`
 // the class. Ego_r.pdb + FableWin.pdb: ??0CPSCUpdateNormal@NParticleEngine@@QAE@XZ (0x00B0C9C0, out of line),
 // ?Alloc@CPSCUpdateNormal@NParticleEngine@@SIPAVCPSCBase@2@XZ (0x00AF7E60, public static __fastcall; RegisterPSCs 0x00AF8170 binds it)
 // and the ten overrides of CPSCBase's pure virtuals. Declarations only: the virtuals override CPSCBase slots, no new slot, no layout change.
 CPSCUpdateNormal();
 static CPSCBase* FABLE_FASTCALL Alloc();
 virtual CPSCBase* DatabaseClone() const;
 virtual CCharString GetClassTypeName() const;
 virtual EPSCType GetComponentType() const;
 virtual EPSCSubType GetComponentSubType() const;
 virtual CCharString GetGUIName() const;
 virtual void InterpolateTo(const CPSCBase&,const float);
 virtual void ReadBinary(CDataInputStream&);
 virtual void WriteBinary(CDataOutputStream&) const;
 virtual void DatabaseCopy(const CPSCBase&);
 virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
#endif
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
    unsigned char DecalEmitterList[0x4];                                  // +0x9c std::list<CIntelligentPointer<CParticleEmitter> >
};
}
FABLE_STATIC_ASSERT_AT(165, sizeof(NParticleEngine::CPSCUpdateNormal) == 0xa0);
#endif
#elif defined(FABLE_RETAIL_CPSCUPDATENORMAL_ALLOC)
#error "FABLE_RETAIL_CPSCUPDATENORMAL_ALLOC is declared on the containers-free view: define FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS too"
#endif // FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS
