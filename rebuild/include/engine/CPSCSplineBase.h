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
};
}
FABLE_STATIC_ASSERT_AT(30,sizeof(NParticleEngine::CPSCSplineBase)==52);
FABLE_STATIC_ASSERT_AT(31,sizeof(NParticleEngine::CPSCSplineBase::CSplinePosParamData)==24);
#endif
#endif
