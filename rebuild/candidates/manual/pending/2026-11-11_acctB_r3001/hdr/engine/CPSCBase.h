#ifndef FABLE_ENGINE_CPSCBASE_H
#define FABLE_ENGINE_CPSCBASE_H
// NParticleEngine::CPSCBase (new class header, chunk 2001, row 0x00AF4900 CParticleSystem::DatabaseSerialise): the particle-system
// component base. RTTI .?AVCPSCBase@NParticleEngine@@ : CParticleUpdateParamCallback<unsigned long,unsigned long> : CBaseClassNonCopyable;
// vtable 0x0129F3F4 (32 slots). Slot order: the FableWin.pdb definition order of the CPSCBase virtual bodies (OnCreate 0x02F10210 ..
// GetApproximateSplineLength 0x02F103C0) fills retail's implemented slots 7..13, 17, 19, 21..31 (CPSCBase bodies 0x00AF29B0..0x00AF2AE0);
// the pure slots 2..6, 14..16, 18, 20 are the overrides every component declares (CPSCRenderSprite 0x0129F67C: 2 DatabaseClone 0x00AFE580,
// 3 GetClassTypeName, 14 InterpolateTo 0x00AF9840, 15 ReadBinary 0x00AFA2B0, 16 WriteBinary 0x00AF9620, 17 DatabaseSerialise 0x00AFBCE0,
// 18 DatabaseCopy 0x00AF9410, 20 GetComponentCost 0x00AF90C0, 28 Initialise 0x00AF9310; the FableWin.pdb CPSCRenderSprite names order
// 4 GetComponentType / 5 GetComponentSubType / 6 GetGUIName). Slot 1: CParticleUpdateParamCallback::GetUpdateParam. Signatures: the
// FableWin.pdb spellings. Members (Ego_r.pdb): ParticleSystem +0x04, UpdateStarted / Initialised bits +0x08. Only DatabaseSerialise
// (slot 17, +0x44: 0x00AF4D44 `call [edx+44h]`) is used by the chunk-2001 row.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
class C3DBoundingSphere;
class C3DVector;
class CCharString;
class CDataInputStream;
class CDataOutputStream;
class CEnginePrimitiveHandle;
class CEnginePrimitivePrepareInfo;
class CIEngine;
class CPersistContext;
class CSymbolMap;
namespace NParticleEngine {
class CEmitterDatabaseSerialiseInfo;
class CParticleSystem;
class CParticleSystemCost;
template<class K,class V> class CParticleUpdateParameters;
enum EPSCType;                                                           // MS forward enums
enum EPSCSubType;
template<class K,class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector&,unsigned long,float,bool,const CParticleUpdateParameters<K,V>&,unsigned long);   // 1
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long,unsigned long> {
public:
    virtual CPSCBase* DatabaseClone() const = 0;                                                       //  2
    virtual CCharString GetClassTypeName() const = 0;                                                  //  3
    virtual EPSCType GetComponentType() const = 0;                                                     //  4
    virtual EPSCSubType GetComponentSubType() const = 0;                                               //  5
    virtual CCharString GetGUIName() const = 0;                                                        //  6
    virtual void OnCreate();                                                                           //  7
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>&);           //  8
    virtual void Update(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,bool&);   //  9
    virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long,unsigned long>&,const unsigned long&);   // 10
    virtual void TransferUpdateParametersToDialog(const CSymbolMap&);                                 // 11
    virtual void UpdateAddParticle(CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,C3DVector,C3DVector,float);   // 12
    virtual void PreparePrimitives(float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,CEnginePrimitiveHandle&);   // 13
    virtual void InterpolateTo(const CPSCBase&,float) = 0;                                             // 14
    virtual void ReadBinary(CDataInputStream&) = 0;                                                    // 15
    virtual void WriteBinary(CDataOutputStream&) const = 0;                                            // 16
    virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);            // 17
    virtual void DatabaseCopy(const CPSCBase&) = 0;                                                    // 18
    virtual bool AddToBoundingSphere(C3DBoundingSphere&,float,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&);   // 19
    virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,float,float) = 0;        // 20
    virtual bool GetParticleLifeSecs(float&);                                                          // 21
    virtual bool GetNumberOfParticles(float&,float);                                                   // 22
    virtual bool GetDecalLifeSecs(float&);                                                             // 23
    virtual bool GetNumberOfDecals(float&,float);                                                      // 24
    virtual void EnterTimelineEnd();                                                                   // 25
    virtual long GetParticleCount();                                                                   // 26
    virtual long GetDecalCount();                                                                      // 27
    virtual bool Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,CEnginePrimitiveHandle&);   // 28
    virtual void ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&);                               // 29
    virtual bool GetUserPoint(C3DVector&) const;                                                       // 30
    virtual float GetApproximateSplineLength();                                                        // 31
    const CParticleSystem* ParticleSystem;                                                             // +0x04
    bool UpdateStarted : 1;                                                                            // +0x08
    bool Initialised : 1;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CPSCBase) == 0xc);
#endif // FABLE_ENGINE_CPSCBASE_H
