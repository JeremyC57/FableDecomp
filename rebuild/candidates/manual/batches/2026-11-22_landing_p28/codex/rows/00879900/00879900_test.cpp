// Deterministic collaborators surround the complete Update body.
#include <stdio.h>
#include <math.h>
#include <vector>
struct C2DCoordI{long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
struct C3DVector{
 float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}
 C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
 float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
 C3DVector& AccurateNormalise(){float m=GetMagnitude();if(m){X/=m;Y/=m;Z/=m;}return *this;}
 C2DCoordI ToC2DCoordI() const {return C2DCoordI((long)X,(long)Y);}
 float GetAccurateMagnitude() const {return GetMagnitude();}
 bool IsNonZero() const {return Y*Y+X*X+Z*Z>0.0001f*0.0001f;}
 C3DVector CrossWithUp() const {return C3DVector(Y,-X,0.0f);}
 C3DVector Cross(const C3DVector& v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
};
struct C2DVector{float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};

struct C2DBoxF{float TLX,TLY,BRX,BRY;C2DBoxF(float a,float b,float c,float d):TLX(a),TLY(b),BRX(c),BRY(d){}};
struct C2DBoxI{C2DBoxI(const C2DCoordI&,long){}};
const float DELTA_NOTIONAL_ZERO=0.0001f;
struct CUserProfileManager{
 bool xi,yi;float sensitivity;static CUserProfileManager& Get(){static CUserProfileManager p;return p;}
 bool IsCameraFPBowYZInverted()const{return yi;}bool IsCameraFPBowXYInverted()const{return xi;}
 float GetCameraSensitivity()const{return sensitivity;}
};
struct CThing;
struct CTCTargetingBase{CThing* candidate;CThing* GetPBestThingForProjectileTargetingLock()const{return candidate;}};
struct CTCHeroRangedAttacks{float pressure,parameter;bool analogue;float GetProjectileTargetingZLockPressure()const{return pressure;}bool IsProjectileTargetingZLock()const{return pressure>0;}bool IsUsingProjectileTargetingAnalogueZoom()const{return analogue;}float GetProjectileTargetingZoomParameter()const{return parameter;}};
struct CTCTargetingPlayer{float GetMaxTargetingDistance()const{return 8;}};
struct CTCHitLocations{C3DVector center;float rad;void GetBoundingSphere(C3DVector& v,float& r,bool)const{v=center;r=rad;}};
struct CThing{bool dead;C3DVector pos;CTCTargetingBase* target;CTCHeroRangedAttacks* ranged;CTCTargetingPlayer* player;CTCHitLocations* hit;
 CThing():dead(false),target(0),ranged(0),player(0),hit(0){}
 const C3DVector& GetPos()const{return pos;}bool IsDead()const{return dead;}
 bool PeekTC(const CTCTargetingBase** p)const{*p=target;return *p!=0;}
 bool PeekTC(const CTCHeroRangedAttacks** p)const{*p=ranged;return *p!=0;}
 bool PeekTC(const CTCTargetingPlayer** p)const{*p=player;return *p!=0;}
 bool PeekTC(const CTCHitLocations** p)const{*p=hit;return *p!=0;}
};
inline bool IS_THING_ALIVE(const CThing* thing){return thing && !thing->IsDead();}
struct CThingCreatureBase : CThing {};
struct CThingFilter_IsNot{const CThing& ignored;explicit CThingFilter_IsNot(const CThing& thing):ignored(thing){}};
struct WorldFixture{
 const CThing* observed;const CThingCreatureBase* creature;int searches;
 WorldFixture():observed(0),creature(0),searches(0){}
 WorldFixture& PeekThingSearchTools(){return *this;}
 template<class T>void GetAllThingsInRoughArea(const C2DBoxI&, const CThingFilter_IsNot&,std::vector<const CThing*>&);
};
template<> inline void WorldFixture::GetAllThingsInRoughArea<const CThingCreatureBase>(const C2DBoxI&,const CThingFilter_IsNot& filter,std::vector<const CThing*>& found) {
 ++searches;observed=&filter.ignored;if(creature && creature!=observed) found.push_back(creature);
}
struct CTools{enum ELOSTest{unused};int rays;bool final_clear,lock_clear;const CThing* blocker;const CThing* foundCreature;
 CTools():rays(0),final_clear(true),lock_clear(true),blocker(0),foundCreature(0){}
 bool IsLineOfSight(const std::vector<const CThing*>& list,const C3DVector&,const C3DVector&,C3DVector& pos,C3DVector&,float& d,const CThing** thing,ELOSTest kind,bool){
 if((int)kind==8 && list.size()==1)foundCreature=list[0];
 ++rays;pos=C3DVector(2,4,8);d=2;*thing=blocker;return (int)kind==8?final_clear:lock_clear;
 }};
struct CameraDefFixture{bool ProjectileCameraFOVZoom;float XAxisDisplacementCoupling,XAxisVelocityDamping,YAxisDisplacementCoupling,YAxisVelocityDamping,ProjectileCameraDefaultTranslationalZoomOffset;};
namespace NCameraModes {
struct CProjectileWeapon{
 static bool InvertY;
 CThing* observed;CameraDefFixture* def;WorldFixture world;CTools tools;C2DVector stick;C3DVector look_direction,position,look;
 float DeltaLockDeltaPhi,DeltaLockDeltaHeightParam,LockDeltaPhi,LockDeltaHeightParam,Phi,HeightParam,DeltaPhi,DeltaHeightParam,ZoomInFOV,MaxZoomInFOV,fov;
 CProjectileWeapon():observed(0),def(0),stick(1,1),look_direction(1,0,0),DeltaLockDeltaPhi(0),DeltaLockDeltaHeightParam(0),LockDeltaPhi(0),LockDeltaHeightParam(0),Phi(0),HeightParam(0),DeltaPhi(0),DeltaHeightParam(0),ZoomInFOV(0.25f),MaxZoomInFOV(0.125f),fov(0){}
 void Update();CThing* GetObservedThing(){return observed;}CameraDefFixture* GetCameraModeDef(){return def;}
 C2DVector GetLeftStickVector(){return stick;}float GetPhiForPosition(const C3DVector& v){return v.Y;}
 float GetHeightParamForPostionWithPhi(const C3DVector& v,float){return v.Z;}
 float GetPhiForTargetedThing(const CThing&){return 0.25f;}float GetHeightParamForTargetedThingWithPhi(const CThing&,float){return 0.5f;}
 C3DVector GetPosFromParams(float p,float h){return C3DVector(p,h,1);}C3DVector GetLookDirectionFromParams(float,float){return look_direction;}
 WorldFixture& GetWorld(){return world;}CTools& GetCamTools(){return tools;}void SetPosition(const C3DVector& v){position=v;}
 void SetObservedPos(const C3DVector& v){look=v;}void SetFOV(float v){fov=v;}
};}
namespace NCameraModes {
bool CProjectileWeapon::InvertY;
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
int main(){
 CThingCreatureBase other;
 CThing owner,target,obstruction;CTCHitLocations hits;hits.center=C3DVector(4,0,0.5f);hits.rad=1;
 CTCTargetingBase targets;CTCHeroRangedAttacks ranged;CTCTargetingPlayer player;
 CameraDefFixture config={true,1,0.5f,1,0.5f,1};owner.target=&targets;owner.ranged=&ranged;
 for(int i=0;i<384;++i){
  NCameraModes::CProjectileWeapon c;c.observed=&owner;c.def=&config;CUserProfileManager& p=CUserProfileManager::Get();
  p.xi=!!(i&1);p.yi=!!(i&2);NCameraModes::CProjectileWeapon::InvertY=!!(i&4);
  p.sensitivity=(float)(i%4-1);targets.candidate=(i%5==0)?0:&target;target.dead=i%7==0;
  target.hit=(i%9)?&hits:0;ranged.pressure=(i%3)?1.0f:0.0f;
  ranged.analogue=!!(i&8);ranged.parameter=(i&16)?-1.0f:0.5f;owner.player=(i&32)?0:&player;
  c.tools.blocker=(i%11)?&obstruction:0;obstruction.dead=!!(i&16);c.tools.final_clear=!!(i&64);c.tools.lock_clear=!!(i&8);
  config.ProjectileCameraFOVZoom=!!(i&32);if(i%4==0)c.look_direction=C3DVector();
  bool locked=targets.candidate&&!target.dead&&target.hit&&ranged.pressure>0;
  c.world.creature=(i&1)?&other:0;
  c.Update();
  if(c.world.searches!=1 || c.world.observed!=&owner || c.tools.foundCreature!=c.world.creature)return 5;
  if(c.tools.rays!=(locked?2:1)) return 1;
  if(!c.tools.final_clear && (c.position.X!=2||c.position.Y!=4||c.position.Z!=8))return 2;
  float f=80.0f/360.0f;float t=ranged.analogue?ranged.parameter:1.0f;
  float expect=(0.25f*3+(t<0?f:(0.125f-f)*t+f))*0.25f;
  if(fabs(c.fov-expect)>0.00001||c.ZoomInFOV!=c.fov)return 3;
  if(!locked){float gain=(p.sensitivity<0?0:p.sensitivity>1?1:p.sensitivity)+0.5f;
   if(fabs(c.DeltaPhi-(p.xi?-1:1)*0.2f*gain*(0.25f*4.5f))>0.00001f)return 4;}
 }
 puts("00879900_TEST PASS");return 0;
}
