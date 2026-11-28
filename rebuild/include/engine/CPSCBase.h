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
#ifndef FABLE_RETAIL_CPARTICLEUPDATEPARAMCALLBACK_IMPLICIT_DTOR
 virtual ~CParticleUpdateParamCallback();
#endif // NF158 opt-in: no user destructor, so the implicit one (no vptr store of its own) chains straight to
       // ~CBaseClassNonCopyable as in retail ??_GCPSCUpdateNormal 0x00B0F990 / ~CPSCUpdateNormal 0x00B0CA90.
};
template<class K,class V>class CParticleUpdateParamDesc:public CBaseClass {
#ifdef FABLE_RETAIL_ORBIT_UPDATE_PARAMS_PASS30
public:
    CParticleUpdateParamDesc(CParticleUpdateParamCallback<K,V>* callback, V index)
        : Callback(callback), Index(index) {}
    // The original descriptor inherits the base destructor.
#else
public: virtual ~CParticleUpdateParamDesc();
#endif
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_CTOR
 // Opt-in (hand3c, row 0x00B103F0 CPSCEmitterGeneric ctor): FableWin.pdb ??0?$CParticleUpdateParamDesc@KK@NParticleEngine@@QAE@XZ
 // 0x02F4E710 (/Od: CBaseClass(), vptr, Callback = 0; Index left alone), inline in retail (0x00B10407 call CBaseClass(),
 // 0x00B10414 `mov [edi+4],eax` with eax = 0, 0x00B10417 vptr 0x0125FEE4). No layout change.
 CParticleUpdateParamDesc():Callback(0){}
#endif
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM
 // Opt-in (wfD6, row 0x00B087C0): FableWin.pdb / Ego_r.pdb ?GetUpdateParam@?$CParticleUpdateParamDesc@KK@NParticleEngine@@QBE_NAAVC3DVector@@M_NABV?$CParticleUpdateParameters@KK@2@K@Z
 // (inline in retail: CPSCSingleSprite::PreparePrimitives 0x00B08AxA `call [edx+4]` through Callback with Index pushed). No layout change.
 bool GetUpdateParam(C3DVector& out,float time,bool local,const CParticleUpdateParameters<K,V>& params,unsigned long flags)const{return Callback->GetUpdateParam(out,Index,time,local,params,flags);}
#endif
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_GETUPDATEPARAM_NESTED
 // Opt-in (hand3c3, row 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere): the same member in its original statement shape.
 // Ego_d.pdb: static member MaxRecursion (const unsigned long), parameters dest / interp / tangent / params / nested, line table
 // 126..133 (assert 274 bytes, the test 11 = `mov eax,[nested] ; cmp eax,[MaxRecursion] ; jae`, the forwarding return 55 = with
 // `nested + 1`, `return false` 2). The limit is 10: the out-of-line copy of the caller, 0x00AF3080, compares the depth with 10
 // and pushes depth + 1. A caller that passes a literal 0 gets the test folded and `push 1` (0x00B0771F). Replaces the flat
 // forwarder above (do not define both).
 bool GetUpdateParam(C3DVector& dest,float interp,bool tangent,const CParticleUpdateParameters<K,V>& params,unsigned long nested)const{
  if(nested<MaxRecursion){
   return Callback->GetUpdateParam(dest,Index,interp,tangent,params,nested+1);
  }
  return false;
 }
private: static const unsigned long MaxRecursion=10;
public:
#endif
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMDESC_ISNULL
 // Opt-in (wfE8 6th agent, row 0x00B10B10): FableWin.pdb ?IsNull@?$CParticleUpdateParamDesc@KK@NParticleEngine@@QBE_NXZ (inline in retail:
 // GenerateParticle tests the Callback word before each GetUpdateParam, 0x00B1150x / 0x00B1178x). No layout change.
 bool IsNull()const{return Callback==0;}
#endif
private: CParticleUpdateParamCallback<K,V>* Callback;V Index;
};
class CPSCBase:public CParticleUpdateParamCallback<unsigned long,unsigned long>{
#ifdef FABLE_RETAIL_CPSCLIGHT_PREPARE_PRIMITIVES_PASS28
public:
    C3DVector GetCurrentPosition(const CParticleUpdateParameters<unsigned long, unsigned long>&,
                                const unsigned long&, float, bool, unsigned long) const;
#endif
public:
#ifdef FABLE_RETAIL_CPSCBASE_CTOR
 // Opt-in (hand3c, rows 0x00B012E0 CPSCRenderMesh / 0x00B103F0 CPSCEmitterGeneric ctors): Ego_r.pdb / FableWin.pdb
 // ??0CPSCBase@NParticleEngine@@QAE@XZ, out of line at 0x00AF2620 (every CPSC* ctor calls it first). Declaration only.
 CPSCBase();
#endif
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
#ifndef FABLE_RETAIL_CPSCBASE_IMPLICIT_DTOR
 virtual ~CPSCBase();
#endif // NF158 opt-in: implicit destructor (see FABLE_RETAIL_CPARTICLEUPDATEPARAMCALLBACK_IMPLICIT_DTOR).
#ifdef FABLE_RETAIL_CPSCBASE_GETUPDATESTARTED_PASS27
public:
 bool GetUpdateStarted() const { return UpdateStarted; }
#endif
private: const CParticleSystem* ParticleSystem;bool UpdateStarted:1;
protected: bool Initialised:1;
};
}
FABLE_STATIC_ASSERT_AT(64, sizeof(NParticleEngine::CPSCBase)==12);
FABLE_STATIC_ASSERT_AT(65, sizeof(NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long>)==12);
#endif
