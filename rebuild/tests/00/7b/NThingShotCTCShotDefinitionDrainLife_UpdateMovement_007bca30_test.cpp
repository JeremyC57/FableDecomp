#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
// Original full shared owner, names and named math/accessor calls from actual donor.
// Leaf: ?UpdateMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ
// Flags: /O1 /Oy-
#define FABLE_RETAIL_SHOT_UPDATEMOVEMENT_PASS26
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_COLLISION_RESPONSE
#define FABLE_RETAIL_CTCPHYSICSBASE_MOVE_ACCESSORS
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE_APPLYACCELERATION
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_CROSS_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#include "engine/NThingShotHelpers.h"
#include "engine/ThingAlive.h"
#include "engine/CMainGameComponent.h"
#pragma optimize("s",on)
#pragma optimize("y",off)

#include <cstdio>
#include <cmath>
#include <cstring>
static int failures=0;
#define CHECK(x) do {if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
unsigned long GFastOneOverSqrtTable[128];
static void InitReciprocalFixture(){
 for(unsigned i=0;i<128;++i){float n=(i<64?2.0f:1.0f)*(1.0f+(i&63)/64.0f);float r=1.0f/(float)std::sqrt(n);unsigned bits;std::memcpy(&bits,&r,4);GFastOneOverSqrtTable[i]=(bits&0x00ffffffu)>>15;}
}
static bool Near(float a,float b,float eps=0.002f){return std::fabs(a-b)<eps;}

struct FixtureTarget{bool alive;bool IsDead()const{return !alive;}C3DVector fire;FixtureTarget():alive(true),fire(0,10,0){}};
static int fire_calls=0,rotate_calls=0;
static bool FABLE_FASTCALL IS_THING_ALIVE(FixtureTarget*const&p){return p&&p->alive;}
struct FixturePhysics{C3DVector Velocity,Position,applied;int sets,accels;FixturePhysics():Velocity(0,1,0),Position(0,0,0),applied(0,0,0),sets(0),accels(0){}const C3DVector&GetVelocity()const{return Velocity;}const C3DVector&GetPosition()const{return Position;}void SetVelocity(const C3DVector&v){++sets;Velocity=v;}void ApplyAcceleration(const C3DVector&v){++accels;applied=v;}};
struct FixtureThing{FixturePhysics*physics;FixturePhysics*GetPhysicsTC(){return physics;}};
struct FixtureDef{float Speed;};
struct FixtureFPS{static long FABLE_FASTCALL GetConstantFPS(){return 20;}};
void C3DVector::Rotate(const C3DVector&axis,float angle){++rotate_calls;float length=(float)std::sqrt(axis.X*axis.X+axis.Y*axis.Y+axis.Z*axis.Z);if(length==0)return;double x=axis.X/length,y=axis.Y/length,z=axis.Z/length,a=angle*6.283185307179586,c=std::cos(a),s=std::sin(a),dot=X*x+Y*y+Z*z;float ox=X,oy=Y,oz=Z;X=(float)(ox*c+(y*oz-z*oy)*s+x*dot*(1-c));Y=(float)(oy*c+(z*ox-x*oz)*s+y*dot*(1-c));Z=(float)(oz*c+(x*oy-y*ox)*s+z*dot*(1-c));}
namespace NThingShot {
static C3DVector FABLE_FASTCALL GetCreatureFireAtPos(const FixtureTarget&t){++fire_calls;return t.fire;}
struct FixtureShot{FixtureThing parent;FixtureTarget*target;FixtureDef def;float MaxCosAngleToTurn,MaxAngleToTurn,Speed;C3DVector Acceleration;bool StraightArrow;FixtureShot(FixturePhysics&p,FixtureTarget*t):target(t),MaxCosAngleToTurn(.9f),MaxAngleToTurn(.1f),Speed(4),Acceleration(0,0,-.2f),StraightArrow(false){parent.physics=&p;def.Speed=80;}FixtureThing&GetParentThing(){return parent;}FixtureTarget*GetPTargetThing()const{return target;}const FixtureDef&GetShotDef()const{return def;}void UpdateMovement();};
}
#define CTCShotDefinitionDrainLife FixtureShot
#define CTCPhysicsBase FixturePhysics
#define CThing FixtureTarget
#define CMainGameComponent FixtureFPS
namespace NThingShot {
void CTCShotDefinitionDrainLife::UpdateMovement()
{
 const CThing* ptarget=GetPTargetThing();
 if(IS_THING_ALIVE(ptarget)) {
  CTCPhysicsBase* ptc_phys=GetParentThing().GetPhysicsTC();
        C3DVector velocity=ptc_phys->GetVelocity();
        velocity.Normalise();
        C3DVector position=ptc_phys->GetPosition();
        C3DVector fire_at_pos=GetCreatureFireAtPos(*ptarget);
        C3DVector to_target=fire_at_pos-position;
        to_target.Normalise();
        float cos_theta=velocity.Dot(to_target);
        if(cos_theta<MaxCosAngleToTurn) {
            C3DVector cross=to_target.Cross(velocity);
            velocity.Rotate(cross,MaxAngleToTurn);
        } else velocity=to_target;
        velocity*=Speed;
        ptc_phys->SetVelocity(velocity);
 }
}
}

#undef CTCShotDefinitionDrainLife
#undef CTCPhysicsBase
#undef CThing
#undef CMainGameComponent
int main(){InitReciprocalFixture();for(int live=0;live<3;++live)for(int straight=0;straight<2;++straight)for(int direction=0;direction<4;++direction){FixturePhysics p;FixtureTarget t;t.alive=live==1;const C3DVector points[]={C3DVector(0,10,0),C3DVector(10,0,0),C3DVector(0,10,10),C3DVector(0,0,0)};t.fire=points[direction];NThingShot::FixtureShot shot(p,live?&t:0);shot.StraightArrow=straight!=0;fire_calls=rotate_calls=0;shot.UpdateMovement();bool update=live==1;CHECK(p.sets==(update?1:0));CHECK(p.accels==0);CHECK(fire_calls==(update?1:0));if(update){CHECK(Near(p.Velocity.X*p.Velocity.X+p.Velocity.Y*p.Velocity.Y+p.Velocity.Z*p.Velocity.Z,16,.08f));if(direction==0){CHECK(rotate_calls==0&&Near(p.Velocity.Y,4));}else CHECK(rotate_calls==1);}else{CHECK(p.Velocity.X==0&&p.Velocity.Y==1&&p.Velocity.Z==0);}if(p.accels)CHECK(p.applied.X==0&&p.applied.Y==0&&p.applied.Z==-.2f);}
 std::puts(failures?"007bca30_TEST FAIL":"007bca30_TEST PASS");return failures?1:0;}