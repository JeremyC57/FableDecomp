// 00879900 NCameraModes::CProjectileWeapon::Update
// Leaf: ?Update@CProjectileWeapon@NCameraModes@@UAEXXZ
// Flags:
// FableWin PUB32/GDATA32 and Ego_d PUB32 identify the private InvertY switch read at 013BAF98.
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_C3DVECTOR_NONZERO
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_PROJECTILE_PRESSURE_PASS28
#define FABLE_RETAIL_PROJECTILE_SETFOV_PASS28
#define FABLE_RETAIL_NCAMERAMODES_CBASE_SETPOSITION_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER_INPUT
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CPROJECTILEWEAPON_INVERTY_PASS28
#define FABLE_RETAIL_PROJECTILE_UPDATE_PASS28
#define FABLE_RETAIL_NCAMERAMODES_CPROJECTILEWEAPON_HEIGHT
#define FABLE_RETAIL_CUSERPROFILEMANAGER_ORIGINAL_TARGETING_QUERY
#define FABLE_RETAIL_CTCTARGETINGPLAYER_MAXDISTANCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_NCAMERAMODES_CBASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_LEFTSTICK
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_SETTERS
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_CAMERAINPUT
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_ZLOCKPRESSURE_INLINE
#define FABLE_RETAIL_CTCTARGETINGPLAYER_MAXTARGETINGDISTANCE
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#include <math.h>
#include "rebuild_abi.h"
#include "engine/ThingAlive.h"
#include "engine/C2DExtentsI.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"

#include "engine/C2DBoxF.h"
class CThing;
#include "engine/NCameraModes_CProjectileWeapon.h"
#include "engine/CCameraModeDef.h"
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCHitLocations.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
#include "engine/CThingFilter_IsNot.h"
#include "fable_user_profile.h"

extern const float DELTA_NOTIONAL_ZERO;                    // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB


namespace NCameraModes {
void CProjectileWeapon::Update()
{
    bool fov_zoom = GetCameraModeDef()->ProjectileCameraFOVZoom;
    const CTCTargetingBase* ptc_targ = 0;
    GetObservedThing()->PeekTC(&ptc_targ);
    const CThing* ptargeted_thing = ptc_targ->GetPBestThingForProjectileTargetingLock();
    C2DVector ls_vec = GetLeftStickVector();
    if (!CUserProfileManager::Get().IsCameraFPBowYZInverted())
        ls_vec.Y = -ls_vec.Y;
    if (CUserProfileManager::Get().IsCameraFPBowXYInverted())
        ls_vec.X = -ls_vec.X;
    if (InvertY)
        ls_vec.Y *= -1.0f;
    const CTCHeroRangedAttacks* ptc_hero_ranged_attacks;
    GetObservedThing()->PeekTC(&ptc_hero_ranged_attacks);
    C2DBoxF view_bounding_box(0.0f,1.0f,0.0f,0.0f);
    float zoom_length = 0.0f;
    const float NOMINAL_FOV = 80.0f / 360.0f;
    float fov_based_scale_factor = ZoomInFOV / NOMINAL_FOV;
    float phi_coupling_constant = fov_based_scale_factor * GetCameraModeDef()->XAxisDisplacementCoupling;
    float phi_damping_constant = GetCameraModeDef()->XAxisVelocityDamping;
    float height_coupling_constant = fov_based_scale_factor * GetCameraModeDef()->YAxisDisplacementCoupling;
    float height_damping_constant = GetCameraModeDef()->YAxisVelocityDamping;
    float cam_sensitivity = CUserProfileManager::Get().GetCameraSensitivity();
    if (cam_sensitivity < 0.0f) cam_sensitivity = 0.0f; else if (cam_sensitivity > 1.0f) cam_sensitivity = 1.0f;
    phi_coupling_constant *= cam_sensitivity + 0.5f;
    height_coupling_constant *= cam_sensitivity + 0.5f;
    const float time_step=0.2f;
    bool lock_on=false;
    const CTCHitLocations* ptc_hit_locations;
    if (IS_THING_ALIVE(ptargeted_thing) && ptc_hero_ranged_attacks->IsProjectileTargetingZLock() && ptargeted_thing->PeekTC(&ptc_hit_locations)) {
        C3DVector sphere_centre;
        float sphere_rad;
        ptc_hit_locations->GetBoundingSphere(sphere_centre, sphere_rad, true);
        float distance_to_target = (sphere_centre - GetObservedThing()->GetPos()).GetAccurateMagnitude();
        zoom_length = distance_to_target;
        C3DVector base_pos = sphere_centre;
        C3DVector line_to_me = GetObservedThing()->GetPos() - base_pos;
        line_to_me.Z=0.0f;
        C3DVector up(0.0f,0.0f,1.0f);
        C3DVector right = up.Cross(line_to_me);
        right.AccurateNormalise();
        C3DVector top_left = base_pos + up * sphere_rad - right * sphere_rad;
        C3DVector bottom_right = base_pos - up * sphere_rad + right * sphere_rad;

        view_bounding_box.TLX = GetPhiForPosition(top_left);
        view_bounding_box.BRX = GetPhiForPosition(bottom_right);
        float height1 = GetHeightParamForPostionWithPhi(top_left, view_bounding_box.TLX);
        float height2 = GetHeightParamForPostionWithPhi(bottom_right, view_bounding_box.BRX);
        view_bounding_box.TLY = height1 > height2 ? height1 : height2;
        view_bounding_box.BRY = height1 < height2 ? height1 : height2;
        if (view_bounding_box.TLY > 1.0f) view_bounding_box.TLY = 1.0f;
        if (view_bounding_box.BRY < 0.0f) view_bounding_box.BRY = 0.0f; else if (view_bounding_box.BRY > view_bounding_box.TLY) view_bounding_box.BRY = view_bounding_box.TLY;
        float target_phi = GetPhiForTargetedThing(*ptargeted_thing);
        float target_height_param = GetHeightParamForTargetedThingWithPhi(*ptargeted_thing, target_phi);
        const float phi_weight = 3.0f;
        Phi = (Phi * phi_weight + target_phi) / (phi_weight + 1.0f);
        const float height_param_weight=3.0f;
        HeightParam = (HeightParam * height_param_weight + target_height_param) / (height_param_weight + 1.0f);
        DeltaLockDeltaPhi += (phi_coupling_constant * ls_vec.X - phi_damping_constant * DeltaLockDeltaPhi) * time_step;
        DeltaLockDeltaHeightParam += (height_coupling_constant * ls_vec.Y - height_damping_constant * DeltaLockDeltaHeightParam) * time_step;
        float test_lock_delta_phi = DeltaLockDeltaPhi * time_step + LockDeltaPhi;
        float test_lock_delta_height_param = DeltaLockDeltaHeightParam * time_step + LockDeltaHeightParam;
        float min_lock_delta_phi = view_bounding_box.TLX - target_phi, max_lock_delta_phi = view_bounding_box.BRX - target_phi;
        if (test_lock_delta_phi < min_lock_delta_phi) test_lock_delta_phi = min_lock_delta_phi; else if (test_lock_delta_phi > max_lock_delta_phi) test_lock_delta_phi = max_lock_delta_phi;
        float min_lock_height_param = view_bounding_box.BRY - target_height_param, max_lock_height_param = view_bounding_box.TLY - target_height_param;
        if (test_lock_delta_height_param < min_lock_height_param) test_lock_delta_height_param = min_lock_height_param; else if (test_lock_delta_height_param > max_lock_height_param) test_lock_delta_height_param = max_lock_height_param;
        DeltaPhi = 0.0f;
        DeltaHeightParam = 0.0f;
        float final_phi = test_lock_delta_phi + target_phi;
        float final_height_param = test_lock_delta_height_param + target_height_param;
        C3DVector test_cam_pos = GetPosFromParams(final_phi, final_height_param);
        C3DVector test_look_direction = GetLookDirectionFromParams(final_phi, final_height_param);
        std::vector<const CThing*> thing_in_a_list;
        thing_in_a_list.push_back(ptargeted_thing);
        const C3DVector& end = test_cam_pos + test_look_direction * (distance_to_target + 2.0f * sphere_rad);
        C3DVector collision_pos, normal_vect;
        float cam_rad;
        const CThing* pthing = 0;
        if (!GetCamTools().IsLineOfSight(thing_in_a_list, test_cam_pos, end, collision_pos, normal_vect, cam_rad, &pthing, (CTools::ELOSTest)15, false)
            && IS_THING_ALIVE(pthing)) {
            LockDeltaPhi = test_lock_delta_phi;
            LockDeltaHeightParam = test_lock_delta_height_param;
        } else {
            DeltaLockDeltaPhi = 0.0f;
            DeltaLockDeltaHeightParam = 0.0f;
        }
    } else {
        Phi += LockDeltaPhi;
        HeightParam = LockDeltaHeightParam + HeightParam;
        LockDeltaPhi = 0.0f;
        LockDeltaHeightParam = 0.0f;
        DeltaLockDeltaPhi = 0.0f;
        DeltaLockDeltaHeightParam = 0.0f;
        DeltaPhi += (phi_coupling_constant * ls_vec.X - phi_damping_constant * DeltaPhi) * time_step;
        DeltaHeightParam += (height_coupling_constant * ls_vec.Y - height_damping_constant * DeltaHeightParam) * time_step;
    }
    Phi += DeltaPhi * time_step;
    HeightParam += DeltaHeightParam * time_step;
    if (HeightParam < 0.0f) HeightParam = 0.0f; else if (HeightParam > 1.0f) HeightParam = 1.0f;
    float phi_to_use = Phi + LockDeltaPhi;
    float height_param_to_use = LockDeltaHeightParam + HeightParam;
    if (height_param_to_use < 0.0f) height_param_to_use = 0.0f; else if (height_param_to_use > 1.0f) height_param_to_use = 1.0f;
    C3DVector cam_pos = GetPosFromParams(phi_to_use, height_param_to_use);
    C3DVector look_direction = GetLookDirectionFromParams(phi_to_use, height_param_to_use);
    float look_distance = 1.0f;
    if (look_direction.IsNonZero()) {
        look_direction.AccurateNormalise();
        const CTCTargetingPlayer* ptc_player_targ=0;
        if (GetObservedThing()->PeekTC(&ptc_player_targ))
            look_distance = ptc_player_targ->GetMaxTargetingDistance();
    }
    C3DVector look_at_pos = cam_pos + look_direction * look_distance;
    C3DVector base_camera_pos(cam_pos);
    float zoom_offset_distance = GetCameraModeDef()->ProjectileCameraDefaultTranslationalZoomOffset;
    if (zoom_length < zoom_offset_distance) zoom_length = zoom_offset_distance;
    C3DVector zoom_offset = look_direction * zoom_offset_distance;
    if (!fov_zoom) {
        float zoom_in_length_param=1.0f;
        zoom_offset += look_direction * (zoom_length-zoom_offset_distance) * zoom_in_length_param;
    }
    float target_zoomin_param = 1.0f;
    if (ptc_hero_ranged_attacks->IsUsingProjectileTargetingAnalogueZoom())
        target_zoomin_param = ptc_hero_ranged_attacks->GetProjectileTargetingZoomParameter();
    float target_zoomin_fov = NOMINAL_FOV;
    if (target_zoomin_param >= 0.0f)
        target_zoomin_fov = (MaxZoomInFOV - target_zoomin_fov) * target_zoomin_param + target_zoomin_fov;
    float weight=3.0f;
    ZoomInFOV = (ZoomInFOV * weight + target_zoomin_fov) / (1.0f + weight);
    SetFOV(ZoomInFOV);
    cam_pos += zoom_offset;
    C3DVector out(look_direction);
    out.Z=0.0f;
    out.AccurateNormalise();
    out = out.CrossWithUp();
    float pop_out_param=height_param_to_use-0.5f;
    const float pop_out_max_amp=0.25f, pop_out_weight=2.0f;
    float mag_pop_out=(float)(exp(-(height_param_to_use * height_param_to_use)*pop_out_weight) * (double)pop_out_max_amp);
    cam_pos += out * mag_pop_out;
    C3DVector from = GetObservedThing()->GetPos();
    from.Z = cam_pos.Z;

    C3DVector occlusion_pos, collision_normal;
    float cam_rad;
    const CThing* phit_thing;
    std::vector<const CThing*> all_creatures;
    GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<const CThingCreatureBase>(C2DBoxI(base_camera_pos.ToC2DCoordI(), 2), CThingFilter_IsNot(*GetObservedThing()), all_creatures);

    if (!GetCamTools().IsLineOfSight(all_creatures, from, cam_pos, occlusion_pos, collision_normal, cam_rad, &phit_thing, (CTools::ELOSTest)8, false))
        cam_pos = occlusion_pos;
    SetPosition(cam_pos);
    SetObservedPos(look_at_pos);
}
} // namespace NCameraModes
