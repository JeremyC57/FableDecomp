#ifndef FABLE_ENGINE_CPSCSPLINEBASE_H
#define FABLE_ENGINE_CPSCSPLINEBASE_H
#if defined(FABLE_RETAIL_CPSCSPLINEBASE_PASS23) && !defined(FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS)
#include "engine/CPSCBase.h"
#include "engine/CCardinalSpline.h"
#include "engine/CCountedPointer.h"
// Layout: both original PDBs prove these named fields and nested types.
// Retail uses12-byte arrays, so the outer fields are+12,+16,+36,+44;
// CSplinePosParamData fields are+0,+4,+8,+12, sizeof24.
namespace NParticleEngine {
class CPSCSplineBase:public CPSCBase {
public:
 class CSplineRandomOffsetData;
 class CSplinePosParamData {
 public:
  CSplinePosParamData(const CCardinalSpline&);
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_UPDATE_PASS26
  // Both original PDBs, donor02f5e230, native00b20ba0.
  void Update(const CParticleUpdateParameters<unsigned long,unsigned long>& params,CCardinalSpline& spline);
#endif
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_POSPARAM_INTERPOLATETO
  // Opt-in (NF393, row 0x00B217D0): Ego_r.pdb / FableWin.pdb (0x02F5E500)
  // ?InterpolateTo@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z, called once, by CPSCSplineBase::InterpolateTo
  // (0x00B22483 inside 0x00B22310). Declaration only, no layout change.
  void InterpolateTo(const CSplinePosParamData& other,float t);
#endif
 private:
  void TransformPosToMaintainShape(C3DVector&,const C3DVector&,const C3DVector&,const C3DVector&,const C3DVector&);
  const CParticleSystem* PParentParticleSystem;
  const CCardinalSpline& Spline;
  bool MaintainSplineShape;
  CArray<unsigned long> SplinePosParam;
 };
 float SplineAnimSpeed;
 CCardinalSpline Spline;
 CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;
 CCountedPointer<CSplinePosParamData> SplinePosParamData;
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_GETCOMPONENTCOST
 // Opt-in (hand3c, row 0x00B05C60 CPSCSingleSprite::GetComponentCost): Ego_r.pdb / FableWin.pdb 0x02F5EFB0
 // ?GetComponentCost@CPSCSplineBase@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z, out of line at
 // 0x00B202C0 (the sprite calls it qualified when UseSplinePoints). Declaration only, no layout change.
 virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
#endif
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS
 // Opt-in (Account B re-port, 2026-11-20, the CPSCSingleSprite rows): CPSCSplineBase's own member functions, Ego_r.pdb names and
 // signatures (?..@CPSCSplineBase@NParticleEngine@@; the derived sprite calls the base forms qualified, e.g. DatabaseCopy 0x00B22530 from
 // 0x00B05EA0). Declarations only: the virtuals override CPSCBase slots, no layout change.
 CPSCSplineBase();
 virtual ~CPSCSplineBase();
 virtual void StartUpdate(const CParticleUpdateParameters<unsigned long,unsigned long>&);
 virtual void InterpolateTo(const CPSCBase&,const float);
 virtual void ReadBinary(CDataInputStream&);
 virtual void WriteBinary(CDataOutputStream&) const;
 virtual void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 virtual void DatabaseCopy(const CPSCBase&);                                                      // 0x00B22530
 virtual bool Initialise(CIEngine*,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,CEnginePrimitiveHandle&);
 virtual float GetApproximateSplineLength();
#ifndef FABLE_RETAIL_CPSCSPLINEBASE_GETCOMPONENTCOST
 virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);   // 0x00B202C0
#endif
 void GetPointAtPos(const CParticleUpdateParameters<unsigned long,unsigned long>&,C3DVector&,float,float);
 void GetPointAtIndex(const CParticleUpdateParameters<unsigned long,unsigned long>&,C3DVector&,long,float);
#endif
};
}
FABLE_STATIC_ASSERT_AT(30,sizeof(NParticleEngine::CPSCSplineBase)==52);
FABLE_STATIC_ASSERT_AT(31,sizeof(NParticleEngine::CPSCSplineBase::CSplinePosParamData)==24);
#endif
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS
// Add-only per-TU opt-in (NF462, 2026-11-26, ruling r460d on r451a / r459d): the class above without STLport. The typed view's
// CCardinalSpline Spline (engine/CCardinalSpline.h -> engine/CArray.h -> stlport/fable_containers.h -> <new>, which adds
// /DEFAULTLIB:"libcp" to the object's .drectve; retail's objects never had it, r450a) keeps its name and offset here as 20 opaque bytes;
// the nested CSplineRandomOffsetData / CSplinePosParamData (whose SplinePosParam is a CArray) are only declared; every other member is
// the typed view's. For TUs that only name the class or derive from it to allocate a subclass (no member construction, destruction or
// spline access). Define it TOGETHER with FABLE_RETAIL_CPSCSPLINEBASE_PASS23; the typed view's opt-ins are not carried.
#ifndef FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#error "FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS needs FABLE_RETAIL_CPSCSPLINEBASE_PASS23"
#endif
#if defined(FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS) || defined(FABLE_RETAIL_CPSCSPLINEBASE_GETCOMPONENTCOST) || defined(FABLE_RETAIL_CPSCSPLINEBASE_UPDATE_PASS26) || defined(FABLE_RETAIL_CPSCSPLINEBASE_POSPARAM_INTERPOLATETO)
#error "FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS cannot be combined with the typed view's opt-ins (use the typed view)"
#endif
#include <stddef.h>
#include "engine/CPSCBase.h"
#include "engine/CCountedPointer.h"
namespace NParticleEngine {
class CPSCSplineBase:public CPSCBase {
public:
 class CSplineRandomOffsetData;
 class CSplinePosParamData;
 float SplineAnimSpeed;                                         // +0x0c
 unsigned char Spline[0x14];                                    // +0x10 CCardinalSpline
 CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData; // +0x24
 CCountedPointer<CSplinePosParamData> SplinePosParamData;       // +0x2c
};
}
FABLE_STATIC_ASSERT_AT(96,sizeof(NParticleEngine::CPSCSplineBase)==52);
FABLE_STATIC_ASSERT_AT(97,offsetof(NParticleEngine::CPSCSplineBase,SplineRandomOffsetData)==36);
#endif // FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS
#endif
