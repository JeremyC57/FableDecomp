#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_C3DVECTOR_SQUARED2D_ONE_EXPRESSION_PASS23
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/GFExp.h"

const float DELTA_NOTIONAL_ZERO=0.0001f;
C3DVector& C3DVector::AccurateNormalise(){
    double mag=sqrt(double(X)*X+double(Y)*Y+double(Z)*Z);
    X=(float)(X/mag);Y=(float)(Y/mag);Z=(float)(Z/mag);return *this;
}
static void check(bool yes){if(!yes)exit(3);}
static bool near(float a,float b){return fabs(double(a)-double(b))<0.00002;}
static void same(const C3DVector& a,const C3DVector& b){check(near(a.X,b.X)&&near(a.Y,b.Y)&&near(a.Z,b.Z));}
static int trace[32],count;
static void event(int code){check(count<32);trace[count++]=code;}
class CCamera {
public:
    C3DVector position,point;float horizontal,vertical,roll;
    void MoveTo(const C3DVector& p){event(11);position=p;}
    void PointAt(const C3DVector& p){event(12);point=p;}
    void SetFOV(float h,float v){event(14);horizontal=h;vertical=v;}
};
namespace NCameraModes {
struct CTools {
    template<class T> T GetRollingAverage(const T& oldVal,const T& newVal,float weight)const{return(oldVal*weight+newVal)/(weight+1);}
};
struct Observed {C3DVector pos;const C3DVector& GetPos()const{return pos;}};
struct ModeDef {float LazyLookAtPosWeight;};
struct DefPtr {ModeDef* p;bool IsNull()const{return p==0;}const ModeDef* operator->()const{return p;}};
struct CamManager {CTools tools;CTools& GetCameraTools(){return tools;}};
struct CBase {
    C3DVector observed,position,afterUpdate,afterComponents,forcedLook,fxPos,fxLook,fxDir;
    C3DVector queryFrom,queryTo,queryHit;
    CamManager manager;CCamera cam;Observed thing;DefPtr def;
    C2DVector RightStickVector,LeftStickVector;
    float fov,roll,z,fxAngle;bool raise,ViewOccluded,visibleAfterUpdate,hit,replaceDir;
    const C3DVector& GetObservedPos()const{return observed;}
    const C3DVector& GetPosition()const{return position;}
    void Update(){event(1);observed=afterUpdate;fov+=1;}
    void UpdateSubComponents(){event(2);observed=afterComponents;roll+=2;z+=1;}
    float GetFOV()const{return fov;}
    float GetRollAngle()const{return roll;}
    float GetObservedPositionZOffset()const{return z;}
    float GetDefaultFOV(){event(3);return 16;}
    float GetDefaultRollAngle(){event(4);return -4;}
    float GetDefaultObservedPositionZOffset(){event(5);return 2;}
    void SetFOV(float x){fov=x;}void SetRollAngle(float x){roll=x;}
    void SetObservedPositionZOffset(float x){z=x;}
    const CTools& GetCamTools()const{return manager.tools;}
    const DefPtr& GetCameraModeDef()const{return def;}
    CamManager& GetCameraManager(){return manager;}
    CCamera& GetCamera(){return cam;}
    const Observed* GetObservedThing()const{return &thing;}
    void UpdateCameraStretchAroundHero(C3DVector& p,C3DVector& look){
        event(6);p.X+=2;look.Y-=1;
    }
    void UpdateCameraOcclusion(C3DVector& p){event(7);p.Y+=1;ViewOccluded=visibleAfterUpdate;}
    void UpdateRaiseUp(C3DVector& p)const{event(8);p.Z+=4;}
    bool IsRaiseUpOnHeroNearby()const{return raise;}
    bool IsCameraOcclusionFromTo(const C3DVector& from,const C3DVector& to,C3DVector& out){
        event(9);queryFrom=from;queryTo=to;if(hit)out=queryHit;return hit;
    }
    void UpdateFixLookDirection(const C3DVector& p,const C3DVector& look,C3DVector& dir,float angle){
        event(10);fxPos=p;fxLook=look;fxDir=dir;fxAngle=angle;if(replaceDir)dir=forcedLook;
    }
    void FrameUpdate();
};
static void SetCameraRoll(CCamera& c,float value){event(13);c.roll=value;}
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
int main(){
    using namespace NCameraModes;
    for(unsigned bits=0;bits<256;++bits)for(int scenario=0;scenario<12;++scenario) {
        count=0;CBase c;ModeDef def;def.LazyLookAtPosWeight=(bits&1)?3.0f:0.0f;c.def.p=(bits&2)?&def:0;
        c.raise=(bits&4)!=0;c.visibleAfterUpdate=(bits&8)!=0;c.hit=(bits&16)!=0;c.replaceDir=(bits&32)!=0;
        c.ViewOccluded=!c.visibleAfterUpdate;
        c.position=C3DVector(1,2,4);c.observed=C3DVector(-2,6,0);c.afterUpdate=C3DVector(2,-6,1);
        c.afterComponents=C3DVector(4,-2,2);c.queryHit=C3DVector(7,4,-2);
        c.thing.pos=(bits&64)?C3DVector(3,3,0):C3DVector(9,-2,0);
        const float small[]={0,0.00006103515625f,0.0001220703125f};
        c.forcedLook=scenario<3?C3DVector(small[scenario],0,0):C3DVector(0,-3,4);
        c.fov=3+scenario;c.roll=2-scenario;c.z=(bits&128)?-3.0f:1.0f;
        c.RightStickVector=c.LeftStickVector=C2DVector(1,-1);
        c.cam.point=C3DVector(111,112,113);c.cam.roll=95;
        float wf=c.def.p?def.LazyLookAtPosWeight:0, f=(16+(c.fov+1)*3)*0.25f;
        float r=(-4+(c.roll+2)*3)*0.25f,z=(2+(c.z+1)*3)*0.25f;
        C3DVector look((-2*wf+4)/(wf+1),(6*wf-2)/(wf+1)-1,2/(wf+1)+z);
        C3DVector base(3,3,4),raised(3,3,8),dir(look.X-3,look.Y-3,look.Z-4);
        bool query=c.raise&&c.visibleAfterUpdate;
        C3DVector finalPos=query?(c.hit?c.queryHit:raised):base;
        const float dx=finalPos.X-c.thing.pos.X,dy=finalPos.Y-c.thing.pos.Y;
        const float angle=query?(float)(0.24f*(1.0-exp(-(dx*dx+dy*dy)*0.2f))):0.24f;
        c.FrameUpdate();check(near(c.fov,f)&&near(c.roll,r)&&near(c.z,z));
        same(c.fxLook,look);same(c.fxDir,dir);same(c.fxPos,finalPos);same(c.cam.position,finalPos);
        if(query){same(c.queryFrom,base);same(c.queryTo,raised);}check(near(c.fxAngle,angle));
        C3DVector aimed=c.replaceDir?c.forcedLook:dir;
        double length=sqrt((double)aimed.X*aimed.X+(double)aimed.Y*aimed.Y+(double)aimed.Z*aimed.Z);
        int expected[16],n=0;for(int i=1;i<=8;++i)expected[n++]=i;
        if(query)expected[n++]=9;expected[n++]=10;expected[n++]=11;
        if(length>0.0001){expected[n++]=12;expected[n++]=13;
            same(c.cam.point,C3DVector(finalPos.X+(float)(aimed.X/length),finalPos.Y+(float)(aimed.Y/length),finalPos.Z+(float)(aimed.Z/length)));
            check(near(c.cam.roll,r));}
        else{same(c.cam.point,C3DVector(111,112,113));check(c.cam.roll==95);}
        expected[n++]=14;check(n==count);for(int j=0;j<n;++j)check(trace[j]==expected[j]);
        check(c.cam.vertical==0&&near(c.cam.horizontal,f));
        check(c.RightStickVector.X==0&&c.RightStickVector.Y==0&&c.LeftStickVector.X==0&&c.LeftStickVector.Y==0);
    }
    puts("0084dc10_TEST PASS");return 0;
}