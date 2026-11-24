// NParticleEngine::CPSCSplineBase::CSplinePosParamData::Update -- update parameterised control points in parent system space.
// Leaf: ?Update@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVCCardinalSpline@@@Z
// Flags: (none)
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_UPDATE_PASS26
#define FABLE_RETAIL_CPARTICLESYSTEM_INVERSE_TRANSLATE_PASS26
#define FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_GETVECTOR_PASS26
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_TOC3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_TOCOORDF_PASS26
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#include "fable_string.h"
#include "engine/CPSCSplineBase.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CMatrix3x4.h"
#include "engine/CParticleSystem.h"
void NParticleEngine::CPSCSplineBase::CSplinePosParamData::Update(const CParticleUpdateParameters<unsigned long,unsigned long>& params,CCardinalSpline& spline)
{
 if(!MaintainSplineShape) {
  for(long c_i=0;c_i<Spline.GetControlPointCount();++c_i) {
   C3DVector temp;
   if(params.GetVectorParam(temp,SplinePosParam[c_i])) {
    C3DVector pt=temp.ToC3DCoordF();
    if(PParentParticleSystem) {
     CMatrix3x4 inv_transform;
     PParentParticleSystem->GetInverseSystemTransformWithTranslate(inv_transform);
     inv_transform.TransformPoint(pt);
    }
    spline.SetControlPoint(c_i,pt.ToC3DVector());
   }
  }
 }
}