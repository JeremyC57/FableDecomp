// 00b20d30 NParticleEngine::CPSCSplineBase::CSplinePosParamData::GetClosestParameterisedControlPoints
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
// Leaf: ?GetClosestParameterisedControlPoints@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@AAE_NABV?$CParticleUpdateParameters@KK@3@JJAAVC3DVector@@111@Z
// Flags: (none)
#define FABLE_RETAIL_CPSCSPLINEBASE_CLOSEST_PASS29
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
bool NParticleEngine::CPSCSplineBase::CSplinePosParamData::GetClosestParameterisedControlPoints(const CParticleUpdateParameters<unsigned long,unsigned long>& params, long start_pos_id, long end_pos_id, C3DVector& start_control_point, C3DVector& start_param_point, C3DVector& end_control_point, C3DVector& end_param_point)
{
    bool found = false;
    while (!found && start_pos_id >= 0) {
        if (SplinePosParam[start_pos_id] == 0) --start_pos_id;
        else found = true;
    }
    if (!found) return false;
    found = false;
    long control_point_count = Spline.GetControlPointCount();
    while (!found && end_pos_id < control_point_count) {
        if (SplinePosParam[end_pos_id] == 0) ++end_pos_id;
        else found = true;
    }
    if (!found) return false;
    bool ret_val = params.GetVectorParam(start_param_point, SplinePosParam[start_pos_id]);
    ret_val &= params.GetVectorParam(end_param_point, SplinePosParam[end_pos_id]);
    if (ret_val) {
        start_control_point = Spline.GetControlPoint(start_pos_id);
        end_control_point = Spline.GetControlPoint(end_pos_id);
        if (PParentParticleSystem) {
            CMatrix3x4 inv_transform;
            PParentParticleSystem->GetInverseSystemTransformWithTranslate(inv_transform);
            C3DVector pt(start_param_point);
            inv_transform.TransformPoint(pt);
            start_param_point = pt.ToC3DCoordF();
            C3DVector pt2(end_param_point);
            inv_transform.TransformPoint(pt2);
            end_param_point = pt2.ToC3DCoordF();
        }
    }
    return ret_val;
}