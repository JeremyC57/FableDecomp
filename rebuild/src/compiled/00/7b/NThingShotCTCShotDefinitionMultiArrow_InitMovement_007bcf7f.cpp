// 007bcf7f NThingShot::CTCShotDefinitionMultiArrow::InitMovement
// Flags: none
// Leaf: ?InitMovement@CTCShotDefinitionMultiArrow@NThingShot@@MAEXXZ
#ifdef _MSC_VER
#pragma optimize("y", off)
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#include <math.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
// Fidelity: the shared SetVelocity COMDAT is speed-compiled with omitted frame pointer in native0066f9b0.
#ifdef _MSC_VER
#pragma optimize("t", on)
#pragma optimize("y", on)
#endif
#include "engine/CTCPhysicsBase.h"
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingMultiArrowShotDef.h"
#include "engine/NThingShot_CTCShotDefinitionMultiArrow.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt,cos)
#endif
#include "engine/GFCos.h"
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction,long accuracy,unsigned long* seed);
void CTCShotDefinitionMultiArrow::InitMovement()
{
    CTCPhysicsBase* ptc_phys=ParentThing->GetPhysicsTC();
    C3DVector fire_at_pos=GetShootAtPos();
    C3DVector line_to_target=fire_at_pos-ptc_phys->GetPosition();
    float distance=line_to_target.GetAccurateMagnitude();
    if(!StraightArrow)
        line_to_target=RandomiseShotDirection(line_to_target,GetShotAccuracyPercentage(),GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up=line_to_target.CrossWithUp().Cross(line_to_target);
    C3DVector right=line_to_target.Cross(up);
    up.AccurateNormalise();
    right.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up,line_to_target));
    float speed=GetShotDef().Speed/CMainGameComponent::GetConstantFPS();
    const CThingShotDef* pt_shot_definition=&GetShotDef();
    CDefPointer<const CThingMultiArrowShotDef> pt_multi_shot_definition;
    pt_shot_definition->GetSubDef(&pt_multi_shot_definition);
    if(!StraightArrow){
        if(distance>pt_multi_shot_definition->MaxDistance)distance=pt_multi_shot_definition->MaxDistance;
        float max_angle=pt_multi_shot_definition->MaxAngle*distance/pt_multi_shot_definition->MaxDistance;
        float angle=-max_angle+GFFloatRandom(max_angle*2.0f,GetWorld().GetWorldSeed());
        line_to_target.Rotate(up,angle/360.0f);
        angle=-max_angle+GFFloatRandom(max_angle,GetWorld().GetWorldSeed());
        line_to_target.Rotate(right,angle/360.0f);
    }
    C3DVector initial_velocity=line_to_target*speed;
    ptc_phys->SetVelocity(initial_velocity);
    if(!StraightArrow){
        Acceleration=(GetShootAtPos()-ptc_phys->GetPosition()-initial_velocity*pt_multi_shot_definition->TimeToReachTarget*CMainGameComponent::GetConstantFPS())*2.0f/
           (pt_multi_shot_definition->TimeToReachTarget*pt_multi_shot_definition->TimeToReachTarget*(CMainGameComponent::GetConstantFPS()*CMainGameComponent::GetConstantFPS()));
    }
    ptc_phys->EnableGravity(false);
    MaxAngleToTurn=pt_multi_shot_definition->MaxAngleToTurnDegs/360.0f;
    MaxCosAngleToTurn=(float)GFCos(MaxAngleToTurn);
}
}