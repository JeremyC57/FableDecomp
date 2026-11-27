// 0084dc10 NCameraModes::CBase::FrameUpdate
// Leaf: ?FrameUpdate@CBase@NCameraModes@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CAMERA_FRAME_UPDATE_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_CAMERA_CROSS_FORWARD_PASS29
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDPOS_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETCAMERA
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
#define FABLE_RETAIL_PROJECTILE_SETFOV_PASS28
#include "engine/CThingGameObject.h"
#include "engine/CCameraModeDef.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/GFExp.h"

#include "engine/GFGetInterpolatedValue.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#ifdef _MSC_VER
#pragma intrinsic(cos, acos, sqrt, exp)
#endif

namespace {
C3DVector __fastcall GetNewCameraPosFromOcclusionParams(const C3DVector& p,const C3DVector& n,float r,const C3DVector&)
{
    C3DVector result=p+n*r; return result;
}
}

namespace NCameraModes {
void FABLE_FASTCALL SetCameraRoll(CCamera& camera, float roll);
inline const CThingGameObject* CBase::GetObservedThing() const { return CameraManager.GetObservedThing(); }
inline CCamera& CBase::GetCamera() { return CameraManager.GetCamera(); }
inline const CTools& CBase::GetCamTools() const { return GetCameraManager().GetCameraTools(); }
inline void CBase::SetRollAngle(float angle) { RollAngle = angle; }
inline void CBase::SetObservedPositionZOffset(float offset) { ObservedPositionZOffset = offset; }

void CBase::UpdateFixLookDirection(const C3DVector& pos, const C3DVector& target, C3DVector& lookDir, float angle) const
{
    bool lookingUp = lookDir.Z > 0.0f;
    float minCos = (float)cos(angle * 6.2831854820251465);
    C3DVector dir = lookDir;
    dir.AccurateNormalise();
    C3DVector flat(dir.X, dir.Y, 0.0f);
    flat.AccurateNormalise();
    C3DVector axis = C3DVector(flat.Y, -flat.X, 0.0f);
    float dot = dir.Z * flat.Z + flat.X * dir.X + flat.Y * dir.Y;
    if (dot < minCos)
        dot = minCos;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float sign = lookingUp ? -1.0f : 1.0f;
    float turn = (float)acos(dot) * 0.15915493667125702f;
    flat.Rotate(axis, turn * sign);
    lookDir = flat;
}

void CBase::UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt) const
{
    if (!IsStretchAroundHero()) return;
    if (GetObservedThing()->GetType() != 2) return;
    if (!GetObservedThing()->PeekPlayer().IsMainPlayer()) return;
    const CThingCreatureBase* hero = static_cast<const CThingCreatureBase*>(GetObservedThing());
    C3DVector offset = lookAt - position;
    C3DVector heroPos = GetObservedThing()->GetPos();
    C3DVector delta(position.X - heroPos.X, position.Y - heroPos.Y, 0.0f);
    float dz = position.Z - heroPos.Z;
    float nomR = hero->GetNominalMeshRadius();
    float curR = hero->GetCurrentMeshRadius();
    float d = (float)sqrt(delta.X * delta.X + delta.Y * delta.Y) - curR;
    if (d < 0.0f) d = 0.0f;
    float blend = (float)(exp((double)((d * d) * -0.01f)));
    float nomH = hero->GetNominalMeshHeight();
    position = delta * GFGetInterpolatedValue(1.0f, (curR / nomR), blend) + heroPos + C3DVector(0.0f, 0.0f, (GFGetInterpolatedValue(1.0f, (GetObservedThingCurrentMeshHeight() / nomH), blend) * dz));
    lookAt = offset + position;
}

bool CBase::IsCameraOcclusionFromTo(const C3DVector& from_pos,const C3DVector& to_pos,C3DVector& occlusion_pos)const
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
    if (fabs(look_dir.Y)>0.0001f) {
        shooter_perp.X=1;
        shooter_perp.Y=-shooter_perp.X*look_dir.X/look_dir.Y;
        shooter_perp.AccurateNormalise();
        shooter_up=look_dir.Cross(shooter_perp);
        shooter_up.AccurateNormalise();
    } else if (fabs(look_dir.X)>0.0001f) {
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

void CBase::FrameUpdate()
{
    C3DVector old_look_at_pos = GetObservedPos();
    Update();
    UpdateSubComponents();
    float new_fov = GetFOV();
    float new_roll_angle = GetRollAngle();
    float new_current_observed_zoffset = GetObservedPositionZOffset();
    new_fov = GetCamTools().GetRollingAverage(new_fov, GetDefaultFOV(), 3.0f);
    SetFOV(new_fov);
    new_roll_angle = GetCamTools().GetRollingAverage(new_roll_angle, GetDefaultRollAngle(), 3.0f);
    SetRollAngle(new_roll_angle);
    new_current_observed_zoffset = GetCamTools().GetRollingAverage(new_current_observed_zoffset, GetDefaultObservedPositionZOffset(), 3.0f);
    SetObservedPositionZOffset(new_current_observed_zoffset);
    C3DVector new_pos = GetPosition();
    C3DVector new_look_pos = GetObservedPos();
    float weight = 0.0f;
    if (!GetCameraModeDef().IsNull())
        weight = GetCameraModeDef()->LazyLookAtPosWeight;
    new_look_pos = GetCameraManager().GetCameraTools().GetRollingAverage(old_look_at_pos, new_look_pos, weight);
    new_look_pos += C3DVector(0, 0, new_current_observed_zoffset);
    UpdateCameraStretchAroundHero(new_pos, new_look_pos);
    UpdateCameraOcclusion(new_pos);
    C3DVector look_dir = new_look_pos - new_pos;
    C3DVector raise_up_pos = new_pos;
    UpdateRaiseUp(raise_up_pos);
    const float DEFAULT_MAX_ANGLE_TO_HORIZONTAL = 0.24f;
    float max_angle_to_horizontal = DEFAULT_MAX_ANGLE_TO_HORIZONTAL;
    C3DVector occ_pos;
    if (IsRaiseUpOnHeroNearby() && ViewOccluded) {
        if (IsCameraOcclusionFromTo(new_pos, raise_up_pos, occ_pos))
            raise_up_pos = occ_pos;
        new_pos = raise_up_pos;
        float sqr_distance_xy = (new_pos - GetObservedThing()->GetPos()).GetSquaredMagnitude2D();
        const float sqr_distance_attenuator = 0.2f;
        max_angle_to_horizontal = DEFAULT_MAX_ANGLE_TO_HORIZONTAL * (1.0 - GFExp(-sqr_distance_xy * sqr_distance_attenuator));
    }
    UpdateFixLookDirection(new_pos, new_look_pos, look_dir, max_angle_to_horizontal);
    CCamera& cam_obj = GetCamera();
    cam_obj.MoveTo(new_pos);
    C3DVector look_vector = look_dir;
    if (look_vector.IsNonZero()) {
        look_vector.AccurateNormalise();
        cam_obj.PointAt(new_pos + look_vector);
        SetCameraRoll(cam_obj, new_roll_angle);
    }
    cam_obj.SetFOV(new_fov, 0.0f);
    RightStickVector = C2DVector(0, 0);
    LeftStickVector = C2DVector(0, 0);
}
}