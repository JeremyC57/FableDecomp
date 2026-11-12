#ifndef FABLE_ENGINE_CPSCBASE_H
#define FABLE_ENGINE_CPSCBASE_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CBaseClass.h"
#include "engine/C3DVector.h"
class CCharString;class CPersistContext;class CSymbolMap;class CEnginePrimitivePrepareInfo;
class CEnginePrimitiveHandle;class CDataInputStream;class CDataOutputStream;class C3DBoundingSphere;class CIEngine;
namespace NParticleEngine {
class CParticleSystem;class CEmitterDatabaseSerialiseInfo;class CParticleSystemCost;
#ifdef _MSC_VER
enum EPSCType;enum EPSCSubType;
#else
enum EPSCType{};enum EPSCSubType{};
#endif
template<class K,class V>class CParticleUpdateParameters;
// Layout: both PDB callback/descriptor types; native descriptor ctor00B15950,
// parent vtable012A002C and DatabaseSerialise bit-field offsets independently agree.
template<class K,class V>class CParticleUpdateParamCallback:public CBaseClassNonCopyable {
public:
 virtual bool GetUpdateParam(C3DVector&,V,float,bool,const CParticleUpdateParameters<K,V>&,unsigned long);
 virtual ~CParticleUpdateParamCallback();
};
template<class K,class V>class CParticleUpdateParamDesc:public CBaseClass {
public: virtual ~CParticleUpdateParamDesc();
private: CParticleUpdateParamCallback<K,V>* Callback;V Index;
};
class CPSCBase:public CParticleUpdateParamCallback<unsigned long,unsigned long>{
public:
 virtual CPSCBase* DatabaseClone() const = 0;
 virtual CCharString GetClassTypeName() const = 0;
 virtual EPSCType GetComponentType() const = 0;
 virtual EPSCSubType GetComponentSubType() const = 0;
 virtual CCharString GetGUIName() const = 0;
 virtual void OnCreate();
 virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&);
 virtual void Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,bool&);
 virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&,const unsigned long&);
 virtual void TransferUpdateParametersToDialog(const CSymbolMap&);
 virtual void UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,C3DVector,C3DVector,float);
 virtual void PreparePrimitives(const float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&);
 virtual void InterpolateTo(const CPSCBase&,const float) = 0;
 virtual void ReadBinary(CDataInputStream&) = 0;
 virtual void WriteBinary(CDataOutputStream&) const = 0;
 virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 virtual void DatabaseCopy(const CPSCBase&) = 0;
 virtual bool AddToBoundingSphere(C3DBoundingSphere&,const float,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&);
 virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float) = 0;
 virtual bool GetParticleLifeSecs(float&);
 virtual bool GetNumberOfParticles(float&,const float);
 virtual bool GetDecalLifeSecs(float&);
 virtual bool GetNumberOfDecals(float&,const float);
 virtual void EnterTimelineEnd();
 virtual long GetParticleCount();
 virtual long GetDecalCount();
 virtual bool Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long, unsigned long>&,CEnginePrimitiveHandle&);
 virtual void ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&);
 virtual bool GetUserPoint(C3DVector&) const;
 virtual float GetApproximateSplineLength();
 virtual ~CPSCBase();
private: const CParticleSystem* ParticleSystem;bool UpdateStarted:1;
protected: bool Initialised:1;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCBase)==12);
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long>)==12);
#endif
