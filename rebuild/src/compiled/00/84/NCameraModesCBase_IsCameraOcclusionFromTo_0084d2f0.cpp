// 0084d2f0 NCameraModes::CBase::IsCameraOcclusionFromTo
// Leaf: ?IsCameraOcclusionFromTo@CBase@NCameraModes@@IBE_NABVC3DVector@@0AAV3@@Z
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_CAMERA_CROSS_FORWARD_PASS29
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CCameraModeDef.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CBase.h"
namespace {
C3DVector __fastcall GetNewCameraPosFromOcclusionParams(const C3DVector& p,const C3DVector& n,float r,const C3DVector&)
{
    C3DVector result=p+n*r; return result;
}
}
inline const NCameraModes::CTools& NCameraModes::CBase::GetCamTools() const { return GetCameraManager().GetCameraTools(); }
bool NCameraModes::CBase::IsCameraOcclusionFromTo(const C3DVector& from_pos,const C3DVector& to_pos,C3DVector& occlusion_pos)const
{
    C3DVector line=to_pos-from_pos;
    if (line.GetSquaredMagnitude()>10000.0f) return false;
    const float DEFAULT_CAMERA_RADIUS=0.01f;
    float camera_radius=DEFAULT_CAMERA_RADIUS;
    float other_camera_radius=0.225f;
    C3DVector look_dir=from_pos-to_pos;
    C3DVector shooter_perp,shooter_up;
    shooter_perp.Z=0;
    bool perform_extended_checks=GetCameraModeDef()->PerformExtendedOcclusionTests;
    if (fabs(look_dir.Y)>DELTA_NOTIONAL_ZERO) {
        shooter_perp.X=1;
        shooter_perp.Y=-shooter_perp.X*look_dir.X/look_dir.Y;
        shooter_perp.AccurateNormalise();
        shooter_up=look_dir.Cross(shooter_perp);
        shooter_up.AccurateNormalise();
    } else if (fabs(look_dir.X)>DELTA_NOTIONAL_ZERO) {
        shooter_perp.Y=1;
        shooter_perp.X=-shooter_perp.Y*look_dir.Y/look_dir.X;
        shooter_perp.AccurateNormalise();
        shooter_up=look_dir.Cross(shooter_perp);
        shooter_up.AccurateNormalise();
    } else perform_extended_checks=false;
    const CTools::ELOSTest std_los_check=CTools::TEST_ALL;
    const CTools::ELOSTest extended_los_check=static_cast<CTools::ELOSTest>(CTools::TEST_THINGS|CTools::TEST_CREATURES);
    bool occlusion=false;
    C3DVector intersect_pos,normal_vector;
    const CThing* phit_thing=0;
    if (!GetCamTools().IsLineOfSight(from_pos,to_pos,intersect_pos,normal_vector,camera_radius,&phit_thing,std_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!perform_extended_checks) {}
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos+shooter_perp*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos-shooter_perp*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos+shooter_up*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    else if (!GetCamTools().IsLineOfSight(to_pos,to_pos-shooter_up*other_camera_radius,intersect_pos,normal_vector,camera_radius,&phit_thing,extended_los_check,true)) {
        occlusion_pos=GetNewCameraPosFromOcclusionParams(intersect_pos,normal_vector,camera_radius,look_dir);
        occlusion=true;
    }
    return occlusion;
}