#include <cmath>
#include <cstdio>
#include <cstdlib>
static void check(bool ok, int line) { if (!ok) { std::printf("Check failed %d\n",line); std::exit(1); } }
#define CHECK(expr) check((expr), __LINE__)
static bool nearf(float a,float b) { return std::fabs(a-b)<0.00004; }
static int turns, range_calls, sign_calls, scatter_calls, sign_roll, fps, accuracy_arg, scatter_mode;
static float input_angle, max_args[2]; static unsigned long world_seed;
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define __fastcall
#endif
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_FIREBALL_MAGNITUDE_HELPER_PASS30
#define FABLE_RETAIL_FIREBALL_NONZERO_HELPER_PASS30
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_NAMED_SQUARED
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#include "engine/C3DVector.h"
const float DELTA_NOTIONAL_ZERO=0.0001f;
void C3DVector::Rotate(const C3DVector& axis,float angle) {
    CHECK(axis.X==0 && axis.Y==0 && axis.Z==1);
    ++turns; input_angle=angle;
    float old_x=X;
    int sign=angle<0 ? -1 : 1;
    X=-sign*Y; Y=sign*old_x;
}
C3DVector& C3DVector::AccurateNormalise() {
    float m=GetAccurateMagnitude();
    if (m!=0) *this*=1.0f/m;
    return *this;
}
static C3DVector scatter_input;
class CRightHandedSet {
public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& u,const C3DVector& f):Up(u),Forward(f){}
};
class CTCPhysicsBase {
public:
    C3DVector pos,last,up,forward,accel;
    int sequence;
    CTCPhysicsBase():sequence(0){}
    const C3DVector& GetPosition()const{return pos;}
    const C3DVector& GetLastPosition()const{return last;}
    void ResetRHSet(const CRightHandedSet& rhs){CHECK(sequence++==0);up=rhs.Up;forward=rhs.Forward;}
    void ApplyAcceleration(const C3DVector& v){CHECK(sequence++==1);accel=v;}
    void EnableGravity(bool value){CHECK(sequence++==2 && !value);}
};
class CThing {
public:
    CTCPhysicsBase physics;
    C3DVector position;
    bool dead;
    const C3DVector& GetPos()const{return position;}
    CTCPhysicsBase* GetPhysicsTC(){return &physics;}
    const CTCPhysicsBase* GetPhysicsTC()const{return &physics;}
};
static bool IS_THING_ALIVE(const CThing* const& t){return t && !t->dead;}
class CMainGameComponent {public:static int GetConstantFPS(){return fps;}};
class CWorld {public:unsigned long* GetWorldSeed(){return &world_seed;}};
static float GFFloatRandom(float amount,unsigned long* seed){ CHECK(seed==&world_seed && range_calls<2);max_args[range_calls++]=amount;return amount*0.5f;}
static unsigned long GFRandom(unsigned long max,unsigned long* seed){CHECK(max==1 && seed==&world_seed && range_calls==2);++sign_calls;return sign_roll;}
class CFloatRange {public:float LowValue,HighValue;};
class CThingShotDef {public:float Speed;CFloatRange ShotOffsetAngleRange,ShotOffsetRadiusRange;};
namespace NThingShot {
static C3DVector RandomiseShotDirection(const C3DVector& aim,long accuracy,unsigned long* seed){
    CHECK(seed==&world_seed);++scatter_calls;scatter_input=aim;accuracy_arg=(int)accuracy;
    return scatter_mode ? C3DVector(0,3,4) : aim;
}
class CTCShotDefinitionDragonFireball {
public:
    CThing shooter;CThing* target;CThingShotDef def;CWorld world;C3DVector aim;float accuracy;
    CThing& GetParentThing(){return shooter;}
    C3DVector GetShootAtPos()const{return aim;}
    const CThingShotDef& GetShotDef()const{return def;}
    const CThing* GetPTargetThing()const{return target;}
    long GetShotAccuracyPercentage()const{return (long)accuracy;}
    CWorld& GetWorld(){return world;}
    void InitMovement();
};
void CTCShotDefinitionDragonFireball::InitMovement()
{
    CTCPhysicsBase* ptc_phys = GetParentThing().GetPhysicsTC();
    C3DVector fire_at_pos = GetShootAtPos();
    C3DVector start_pos = GetParentThing().GetPos();
    C3DVector line_to_target = fire_at_pos - start_pos;
    float shot_speed = GetShotDef().Speed / CMainGameComponent::GetConstantFPS();
    const CThing* ptarget_thing = GetPTargetThing();
    if (IS_THING_ALIVE(ptarget_thing)) {
        C3DVector target_pos = ptarget_thing->GetPos();
        C3DVector target_velocity = ptarget_thing->GetPhysicsTC()->GetPosition() - ptarget_thing->GetPhysicsTC()->GetLastPosition();
        float distance_to_target = (target_pos - start_pos).GetAccurateMagnitude();
        float time_to_get_to_target = 0.0f;
        if (GetShotDef().Speed > DELTA_NOTIONAL_ZERO)
            time_to_get_to_target = distance_to_target / shot_speed;
        if (!target_velocity.IsNonZero()) {
            target_velocity.Initialise(0.0f, 1.0f, 0.0f);
            target_velocity *= 0.1f;
        }
        float max_deviation = GetShotDef().ShotOffsetRadiusRange.HighValue;
        float min_deviation = GetShotDef().ShotOffsetRadiusRange.LowValue;
        float deviation = GFFloatRandom(max_deviation - min_deviation, GetWorld().GetWorldSeed()) + min_deviation;
        float max_angle_deviation = GetShotDef().ShotOffsetAngleRange.HighValue;
        float min_angle_deviation = GetShotDef().ShotOffsetAngleRange.LowValue;
        float angle_deviation = GFFloatRandom(max_angle_deviation - min_angle_deviation, GetWorld().GetWorldSeed()) + min_angle_deviation;
        float up_or_down = GFRandom(1, GetWorld().GetWorldSeed()) == 0 ? -1.0f : 1.0f;
        target_velocity.Rotate(C3DVector(0.0f,0.0f,1.0f), angle_deviation * up_or_down);
        target_velocity *= deviation;
        C3DVector projected_target_pos = target_pos + target_velocity * time_to_get_to_target;
        line_to_target = projected_target_pos - start_pos;
    }
    line_to_target = RandomiseShotDirection(line_to_target, GetShotAccuracyPercentage(), GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up = line_to_target.CrossWithUp().Cross(line_to_target);
    up.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up,line_to_target));
    float speed = shot_speed;
    C3DVector applied_acceleration = line_to_target * speed;
    ptc_phys->ApplyAcceleration(applied_acceleration);
    ptc_phys->EnableGravity(false);
}
}
static void vectorcheck(const C3DVector& actual,double x,double y,double z){
    CHECK(nearf(actual.X,(float)x) && nearf(actual.Y,(float)y) && nearf(actual.Z,(float)z));
}
int main(){
    const float rates[]={-1.0f,0.0f,DELTA_NOTIONAL_ZERO,2.0f,8.0f};
    const float movement[]={0.0f,DELTA_NOTIONAL_ZERO*0.5f,DELTA_NOTIONAL_ZERO,2.0f};
    int cases=0;
    for(int t=0;t<3;t++) for(int k=0;k<5;k++) for(int m=0;m<4;m++)
    for(int fr=1;fr<=2;fr++) for(int sign=0;sign<2;sign++) for(int sc=0;sc<2;sc++){
        NThingShot::CTCShotDefinitionDragonFireball obj;CThing target;
        turns=range_calls=sign_calls=scatter_calls=0;sign_roll=sign;scatter_mode=sc;fps=fr;
        target.dead=t==1;obj.target=t==0?0:&target;obj.shooter.position=C3DVector(1,2,3);
        obj.aim=C3DVector(4,6,3);target.position=C3DVector(7,10,3);
        target.physics.pos=C3DVector(movement[m],0,0);target.physics.last=C3DVector(0,0,0);
        obj.accuracy=17.75f;obj.def.Speed=rates[k];
        obj.def.ShotOffsetAngleRange.LowValue=1;obj.def.ShotOffsetAngleRange.HighValue=5;
        obj.def.ShotOffsetRadiusRange.LowValue=2;obj.def.ShotOffsetRadiusRange.HighValue=6;
        obj.InitMovement();
        CHECK(obj.shooter.physics.sequence==3 && scatter_calls==1 && accuracy_arg==17);
        const int live=t==2;
        CHECK(turns==live && range_calls==2*live && sign_calls==live);
        double x=3,y=4,z=0;
        const double speed=(double)rates[k]/fr;
        if(live){
            CHECK(max_args[0]==4 && max_args[1]==4 && nearf(input_angle,sign?3.0f:-3.0f));
            const double flight=rates[k]>DELTA_NOTIONAL_ZERO?10.0/speed:0;
            const double side=sign?1:-1;
            x=6;y=8;
            if(movement[m]<=DELTA_NOTIONAL_ZERO)x-=side*0.1*4*flight;
            else y+=side*movement[m]*4*flight;
        }
        vectorcheck(scatter_input,x,y,z);
        if(sc){x=0;y=3;z=4;}
        double len=std::sqrt(x*x+y*y+z*z);x/=len;y/=len;z/=len;
        vectorcheck(obj.shooter.physics.forward,x,y,z);
        vectorcheck(obj.shooter.physics.accel,x*speed,y*speed,z*speed);
        const double horizontal=std::sqrt(x*x+y*y);
        vectorcheck(obj.shooter.physics.up,-x*z/horizontal,-y*z/horizontal,horizontal);
        ++cases;
    }
    CHECK(cases==480);
    std::puts("007bcc40_TEST PASS");return 0;
}