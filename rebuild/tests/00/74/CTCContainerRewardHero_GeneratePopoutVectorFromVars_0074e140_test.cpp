// Standalone test of the current production function body with observed callees stubbed.
#include <assert.h>
#include <stdio.h>
#include <math.h>
#define FABLE_FASTCALL
class C3DVector { public: float X,Y,Z; C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
static unsigned long seed;
static float facing_value,random_value,random_range;
static long adjusted_row;
class CWorld { public: unsigned long* GetWorldSeed(){return &seed;} };
static CWorld world;
static CWorld& PeekWorld(){return world;}
class CTCPhysicsBase { public: float GetFacingAngleXY(){return facing_value;} };
static CTCPhysicsBase physics;
class CThing { public: CTCPhysicsBase* GetPhysicsTC(){return &physics;} };
class CTCContainerRewardHero { public:
    CThing* ParentThing;
    C3DVector GeneratePopoutVectorFromVars(long,long,long,float)const;
    void AdjustTrajectoryVector(long row,C3DVector& direction)const {adjusted_row=row;direction.Z+=(float)row;}
};
const double TWO_PI=6.28318530717958647692;
float GFFloatRandom(float range,unsigned long* given_seed){assert(given_seed==&seed);random_range=range;return random_value;}
C3DVector CTCContainerRewardHero::GeneratePopoutVectorFromVars(long index, long count, long row, float spread) const
{
    float offset = ((float)index / (float)count * 2.0f - 1.0f) * (spread * 0.5f);
    float facing = ParentThing->GetPhysicsTC()->GetFacingAngleXY();
    float step = 1.0f / (float)count;
    if (step > 1.0f / 24.0f)
        step = 1.0f / 24.0f;
    float angle = GFFloatRandom(step, PeekWorld().GetWorldSeed()) - step * 0.5f + facing + offset;
    float cosPitch = (float)cos(0.0);
    double yaw = angle * TWO_PI;
    C3DVector direction((float)(sin(yaw) * cosPitch), (float)(cos(yaw) * cosPitch), (float)(sin(0.0)));
    AdjustTrajectoryVector(row, direction);
    return direction;
}


static void near(float a,float b){assert(fabs(a-b)<0.00001f);}
int main(){
    CThing thing; CTCContainerRewardHero container;container.ParentThing=&thing;
    facing_value=0.0f;random_value=(1.0f/24.0f)*0.5f;
    C3DVector first=container.GeneratePopoutVectorFromVars(0,8,3,0.0f);
    near(random_range,1.0f/24.0f);near(first.X,0.0f);near(first.Y,1.0f);near(first.Z,3.0f);assert(adjusted_row==3);
    facing_value=0.25f;random_value=(1.0f/32.0f)*0.5f;
    C3DVector second=container.GeneratePopoutVectorFromVars(16,32,0,0.5f);
    near(random_range,1.0f/32.0f);near(second.X,1.0f);near(second.Y,0.0f);near(second.Z,0.0f);
    facing_value=0.0f;random_value=(1.0f/24.0f)*0.5f;
    C3DVector third=container.GeneratePopoutVectorFromVars(0,24,2,0.5f);
    near(third.X,-1.0f);near(third.Y,0.0f);near(third.Z,2.0f);
    puts("0074e140_TEST PASS");return 0;
}