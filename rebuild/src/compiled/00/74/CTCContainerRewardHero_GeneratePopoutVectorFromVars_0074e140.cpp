// CTCContainerRewardHero::GeneratePopoutVectorFromVars(long index, long count, long row, float spread) const -- the launch direction of the index-th of count reward items popping out of a container: spread across the arc around the facing angle, jittered, then adjusted for the row's trajectory
// Leaf: ?GeneratePopoutVectorFromVars@CTCContainerRewardHero@@ABE?AVC3DVector@@JJJM@Z
// Retail: 0x0074e140, 245 bytes, const thiscall member with hidden C3DVector return buffer.
// Flags:
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sin,cos)
#endif
#include "engine/ETCInterfaceType.h"
#include "engine/CTCContainerRewardHero.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
extern const double TWO_PI;
float FABLE_FASTCALL GFFloatRandom(float range,unsigned long* seed);
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