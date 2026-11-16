#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPSCORBIT_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPSCORBIT_H
#include "engine/CPSCBase.h"
class CMatrix3x4;
namespace NParticleEngine {
// Both original complete PDB records: concrete class372bytes and nested V-keyed settings/state.
class CPSCOrbit:public CPSCBase {
public:
 class COrbitSettings {
 public:
  COrbitSettings():Type(0),Enabled(false),Radius(1),Expand(0),CycleFlag(false),CycleTime(5),SqueezeScale(1),SqueezeAngle(0),RotateSpeed(.25f),RotateStart(0),RotateSpeedRandom(0),RotateStartRandom(0){}
  long Type;bool Enabled;float Radius,Expand;bool CycleFlag;
  float CycleTime,SqueezeScale,SqueezeAngle,RotateSpeed,RotateStart,RotateSpeedRandom,RotateStartRandom;
 };
 class COrbitState {
 public:
  COrbitState();
  void Initialise(const COrbitSettings&,unsigned long*);
  void Update(const COrbitSettings&,CMatrix3x4&,float);
  C3DVector GetSplinePosition(float)const;
  float NextAngle,RotateSpeedRandomValue;
  C3DVector FromPosition,ToPosition,NextPosition,FromTangent,ToTangent;
 };
 CPSCOrbit();
 virtual ~CPSCOrbit();
 static CPSCBase* FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StaticGetClassTypeName();
 static EPSCType FABLE_FASTCALL StaticGetComponentType();
 static EPSCSubType FABLE_FASTCALL StaticGetComponentSubType();
 static CCharString FABLE_FASTCALL StaticGetGUIName();
 virtual CCharString GetClassTypeName() const;
 virtual EPSCType GetComponentType() const;
 virtual EPSCSubType GetComponentSubType() const;
 virtual CCharString GetGUIName() const;
 virtual CPSCBase* DatabaseClone() const;
 virtual void InterpolateTo(const CPSCBase&,const float);
 virtual void DatabaseCopy(const CPSCBase&);
 virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 virtual void ReadBinary(CDataInputStream&);
 virtual void WriteBinary(CDataOutputStream&) const;
 virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>&,const unsigned long&);
 virtual bool GetUpdateParam(C3DVector&,unsigned long,float,bool,const CParticleUpdateParameters<unsigned long,unsigned long>&,unsigned long);
 virtual void StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>&);
 virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
 unsigned long CentreParam;
 COrbitSettings Orbits[3];
 COrbitState OrbitStates[3];
 long LifeTimer;
 float LifeTimeSecs;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCOrbit)==372);
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCOrbit::COrbitSettings)==48);
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCOrbit::COrbitState)==68);
#endif
