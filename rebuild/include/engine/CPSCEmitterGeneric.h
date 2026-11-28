#ifndef FABLE_ENGINE_CPSCEMITTERGENERIC_H
#define FABLE_ENGINE_CPSCEMITTERGENERIC_H
#include "engine/CPSCBase.h"
#include "engine/CCountedPointer.h"
class CCardinalSpline;
namespace NParticleEngine {
// Layout: both PDBs prove all private fields and bit widths; native00B13530,
// ReadBinary00B149D0 and WriteBinary00B12EF0 use the same offsets, size0x50.
class CPSCEmitterGeneric:public CPSCBase {
public: virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
#ifdef FABLE_RETAIL_CPSCEMITTERGENERIC_COLLECT_PARAMS_PASS30
    // Both original symbol images and retail slot 10 agree on this override.
    virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>&,
                                         const unsigned long&);
#endif
#ifdef FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
 // Opt-in (hand3c, row 0x00B103F0): Ego_r.pdb 0x00AD1150 / FableWin.pdb 0x02F47CE0 ??0CPSCEmitterGeneric@NParticleEngine@@QAE@XZ.
 // Declaration only, no layout change.
 CPSCEmitterGeneric();
#endif
#ifdef FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC
 // Opt-in (NF462, ruling r460d; the engine/NParticleEngine_CPSCOrbit.h pattern): the class factory and the members a TU needs to `new`
 // the class (the ctor comes from FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR above). Ego_r.pdb + FableWin.pdb
 // ?Alloc@CPSCEmitterGeneric@NParticleEngine@@SIPAVCPSCBase@2@XZ (0x00AF7ED0, public static __fastcall; RegisterPSCs 0x00AF8170 binds it)
 // and the ten overrides of CPSCBase's pure virtuals (DatabaseClone 0x00B15B00 = vtable 0x012A002C slot 2, its only retail reference).
 // Declarations only: the virtuals override CPSCBase slots, no new slot, no layout change.
#ifndef FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#error "FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC needs FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR (the constructor declaration)"
#endif
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
private:
    CParticleUpdateParamDesc<unsigned long,unsigned long> EmitterPosParamDesc;       // +0x0c
    CCountedPointer<CCardinalSpline> Spline;            // +0x18
    unsigned long EmitterPosParam;                      // +0x20
    unsigned long DirectionParamName;                   // +0x24
    long LifeTimer;                                     // +0x28
    float NextGenerateTime;                             // +0x2c
    unsigned long NoParticlesToStart : 10;              // +0x30
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;      // +0x34
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;                     // +0x38
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;                 // +0x3c
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;                 // +0x40
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                        // +0x44
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;             // +0x48
    unsigned long StartedTimer : 1;                     // +0x4c
    unsigned long FinishedGenerating : 1;
#ifdef FABLE_RETAIL_CPSCEMITTERGENERIC_GENERATEPARTICLE
    // Retail opt-in (wfE8 6th agent, row 0x00B10B10): Ego_r.pdb / FableWin.pdb private ?GenerateParticle@CPSCEmitterGeneric@NParticleEngine@@
    // AAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@M@Z and its same-TU callees ?ChangeOrientation@..@@AAE?AVC3DVector@@ABV3@@Z
    // (0x00B10360, landed) and ?GetRandomRadius@..@@ABEMPAK@Z (0x00B102C0, landed). Declarations only; no layout change.
    C3DVector ChangeOrientation(const C3DVector& v);                                   // 0x00B10360
    float GetRandomRadius(unsigned long* seed) const;                                  // 0x00B102C0
    void GenerateParticle(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, float creationTime);
#endif
 };
}
FABLE_STATIC_ASSERT_AT(53, sizeof(NParticleEngine::CPSCEmitterGeneric)==0x50);
#endif
