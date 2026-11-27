// Leaf: ?GetCameraPosition@CZTargetNew@NCameraModes@@AAE?AVC3DVector@@XZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_ISDEAD_DIRECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#include "engine/C3DVector.h"
#include "engine/CThingGameObject.h"
#include "engine/NCameraModes_CZTargetNew.h"
#include "engine/CCameraModeDef.h"
#include "engine/ThingAlive.h"
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/GFAngles.h"

C3DVector NCameraModes::CZTargetNew::GetCameraPosition()
{
    C3DVector base_pos=GetObservedThing()->GetPos();
    bool keep_to_anchor_pos=false;
    if(FramesForTransition>0) {
        --FramesForTransition;
        keep_to_anchor_pos=true;
        Theta+=DeltaTheta;
        BaseHeightOffset+=DeltaBaseHeightOffset;
        BaseOffsetXY+=DeltaBaseOffsetXY;
    }
    if(!IS_THING_ALIVE(PTargetedThing))keep_to_anchor_pos=true;
    C3DVector base_thing_anchor=base_pos+C3DVector(0,0,BaseHeightOffset);
    C3DVector anchor_pos=base_thing_anchor+(XAxis*(float)GFCos(Theta)+YAxis*(float)GFSin(Theta))*BaseOffsetXY;
    if(keep_to_anchor_pos)return anchor_pos;
    C3DVector cam_pos=GetPosition();
    C3DVector line_to_cam=cam_pos-anchor_pos;line_to_cam.Z=0;
    float dist_x=line_to_cam.Dot(XAxis);
    const float max_dist_x=GetCameraModeDef()->ZTargetStringLengthToObservedThing;
    float dist_y=line_to_cam.Dot(YAxis);
    const float max_dist_y=GetCameraModeDef()->ZTargetStringLengthPerpendicularToObservedThing;
    if(fabs(dist_x)>max_dist_x)dist_x=GFGetSign(dist_x)*max_dist_x;
    if(fabs(dist_y)>max_dist_y)dist_y=GFGetSign(dist_y)*max_dist_y;
    cam_pos=anchor_pos+XAxis*dist_x+YAxis*dist_y;
    C3DVector obsthing_to_camera=cam_pos-base_pos;obsthing_to_camera.Z=0;
    float push_sqr_mag=obsthing_to_camera.GetSquaredMagnitude();
    if(push_sqr_mag<GFSqr(CageRadius))
        cam_pos=base_thing_anchor+(obsthing_to_camera*CageRadius)/GFRoot(push_sqr_mag);
    return cam_pos;
}