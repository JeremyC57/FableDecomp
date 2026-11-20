#ifndef FABLE_ENGINE_CPSCSPLINEBASE_H
#define FABLE_ENGINE_CPSCSPLINEBASE_H
#ifdef FABLE_RETAIL_CPSCSPLINEBASE_PASS23
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
#endif
