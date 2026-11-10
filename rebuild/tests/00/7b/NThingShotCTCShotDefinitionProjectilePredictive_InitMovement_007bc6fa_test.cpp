// Standalone C++03 test: selected function body, real vector and pointer accessor bodies, external callees stubbed.
#include <assert.h>
#include <stdio.h>
#include <math.h>
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    const C3DVector operator*(float scale)const{return C3DVector((X*scale),Y*scale,Z*scale);}
    const C3DVector operator^(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    float GetSquaredMagnitude()const{return (X*X+Y*Y+Z*Z);}
    float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    C3DVector& AccurateNormalise();
};
static int normalise_calls;
C3DVector& C3DVector::AccurateNormalise(){++normalise_calls;float length=GetMagnitude();assert(length>0);X/=length;Y/=length;Z/=length;return *this;}
class CRightHandedSet {
protected: C3DVector Up,Forward;
public:
    CRightHandedSet(const C3DVector& up,const C3DVector& forward):Up(up),Forward(forward){}
    const C3DVector& GetUp()const{return Up;}
    const C3DVector& GetForward()const{return Forward;}
};
class CTCPhysicsBase {
public:
    C3DVector Position,OldPosition,acceleration,up,forward;
    int reset_calls,acceleration_calls,gravity_calls;
    bool gravity;
    CTCPhysicsBase():reset_calls(0),acceleration_calls(0),gravity_calls(0),gravity(true){}
    const C3DVector& GetPosition()const{return Position;}
    const C3DVector& GetLastPosition()const{return OldPosition;}
    void ResetRHSet(const CRightHandedSet& set){assert(!acceleration_calls);++reset_calls;up=set.GetUp();forward=set.GetForward();}
    void ApplyAcceleration(const C3DVector& value){assert(reset_calls==1);++acceleration_calls;acceleration=value;}
    void EnableGravity(bool value){assert(acceleration_calls==1);++gravity_calls;gravity=value;}
};
class CBaseObject {public: virtual ~CBaseObject(){} };
class CThing:public CBaseObject {
public:
    CTCPhysicsBase* PhysicsTC; bool alive; mutable int position_calls;
    CThing():PhysicsTC(0),alive(true),position_calls(0){}
    CTCPhysicsBase* GetPhysicsTC(){return PhysicsTC;}
    const CTCPhysicsBase* GetPhysicsTC()const{return PhysicsTC;}
    const C3DVector& GetPos()const{++position_calls;return PhysicsTC->Position;}
};
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem; mutable int get_calls;
    CBaseIntelligentPointer():PItem(0),get_calls(0){}
    CBaseObject* GetPItem()const{++get_calls;return PItem;}
};
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer {
public: T* Get()const{return static_cast<T*>(GetPItem());}
};
class CDefPointeeBase {public: virtual ~CDefPointeeBase(){} };
class CThingShotDef:public CDefPointeeBase {public: float Speed;};
template<class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    T* Get()const{return static_cast<T*>(Object);}
    T* operator->()const{return Get();}
};
class CMainGameComponent {
public: static long ConstantFPS; static long GetConstantFPS(){return ConstantFPS;}
};
long CMainGameComponent::ConstantFPS=4;
static int live_calls;
template<class T> bool IS_THING_ALIVE(const T& thing){++live_calls;return thing && thing->alive;}
namespace NThingShot {
class CTCShotDefinitionBase {
public:
    CThing* ParentThing;
    CIntelligentPointer<const CThing> PTargetThing;
    CIntelligentPointer<CThing> PShooter;
    CDefPointer<const CThingShotDef> PShotDef;
    float AccuracyPercentage;
    C3DVector target_point;
    C3DVector GetShootAtPos()const{return target_point;}
};
class CTCShotDefinitionProjectile:public CTCShotDefinitionBase {};
class CTCShotDefinitionProjectilePredictive:public CTCShotDefinitionProjectile {public:void InitMovement();};
}
namespace NThingShot {
void CTCShotDefinitionProjectilePredictive::InitMovement()
{
    CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    C3DVector target = GetShootAtPos();
    if (IS_THING_ALIVE(PTargetThing.Get()) && IS_THING_ALIVE(PShooter.Get())) {
        C3DVector motion = PTargetThing.Get()->GetPos() - PTargetThing.Get()->GetPhysicsTC()->GetLastPosition();
        const CThing* targetThing = PTargetThing.Get();
        float time = (targetThing->GetPos() - PShooter.Get()->GetPos()).GetMagnitude() / PShotDef->Speed * 3.0f;
        C3DVector lead = motion * time;
        target += lead * ((long)AccuracyPercentage * 0.01f);
    }
    C3DVector dir = target - physics->GetPosition();
    dir.AccurateNormalise();
    C3DVector side = C3DVector(dir.Y, -dir.X, 0.0f) ^ dir;
    side.AccurateNormalise();
    physics->ResetRHSet(CRightHandedSet(side, dir));
    C3DVector velocity = dir * (PShotDef->Speed / CMainGameComponent::GetConstantFPS());
    physics->ApplyAcceleration(velocity);
    physics->EnableGravity(false);
}
}

static void near(float a,float b){assert(fabs(a-b)<0.00001f);}
static void check(bool target_alive,bool shooter_alive,float accuracy){
    CTCPhysicsBase shot_physics,target_physics,shooter_physics;
    shot_physics.Position=C3DVector(0,0,0);
    target_physics.Position=C3DVector(8,4,0);target_physics.OldPosition=C3DVector(8,3,0);
    shooter_physics.Position=C3DVector(4,4,0);
    CThing shot,target,shooter;
    shot.PhysicsTC=&shot_physics;target.PhysicsTC=&target_physics;shooter.PhysicsTC=&shooter_physics;
    target.alive=target_alive;shooter.alive=shooter_alive;
    CThingShotDef def;def.Speed=8.0f;
    NThingShot::CTCShotDefinitionProjectilePredictive movement;
    movement.ParentThing=&shot;movement.PTargetThing.PItem=&target;movement.PShooter.PItem=&shooter;
    movement.PShotDef.Object=&def;movement.AccuracyPercentage=accuracy;movement.target_point=C3DVector(2,0,0);
    live_calls=0;normalise_calls=0;movement.InitMovement();
    bool lead=target_alive && shooter_alive;
    float y=lead?(float)(long)accuracy*0.015f:0.0f;
    float length=(float)sqrt(4.0f+y*y);
    near(shot_physics.forward.X,2.0f/length);near(shot_physics.forward.Y,y/length);near(shot_physics.forward.Z,0);
    near(shot_physics.acceleration.X,4.0f/length);near(shot_physics.acceleration.Y,2.0f*y/length);near(shot_physics.acceleration.Z,0);
    near(shot_physics.up.X,0);near(shot_physics.up.Y,0);near(shot_physics.up.Z,1);
    assert(normalise_calls==2 && shot_physics.reset_calls==1 && shot_physics.acceleration_calls==1);
    assert(shot_physics.gravity_calls==1 && !shot_physics.gravity);
    assert(live_calls==(target_alive?2:1));
    assert(movement.PTargetThing.get_calls==(lead?4:1));
    assert(movement.PShooter.get_calls==(lead?2:target_alive?1:0));
    assert(target.position_calls==(lead?2:0) && shooter.position_calls==(lead?1:0));
}
int main(){
    check(false,true,50.75f);check(true,false,50.75f);
    check(true,true,0.0f);check(true,true,50.75f);check(true,true,-50.75f);check(true,true,100.0f);
    puts("007bc6fa_TEST PASS");return 0;
}