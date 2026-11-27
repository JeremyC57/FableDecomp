// 008633b0 CThingBodyReorienter::ReorientBodyYZForSlopes
// Leaf: ?ReorientBodyYZForSlopes@CThingBodyReorienter@@QAEXXZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#include "fable_assert.h"
#define FABLE_RETAIL_BODY_REORIENT_SLOPE_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBodyReorienter.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include <math.h>
namespace NGlobalConsole { extern bool UseBodyReorientationYZ; }
namespace { float MAX_DOT_FOR_ANGLE_YZ_CHANGE = 0.45f; }
extern const CCharString HEAD_POS_HELPER_NAME, EYE_LEFT_DUMMY_OBJECT_NAME, MAIN_FOCAL_POSITION_NAME;
float __fastcall GFGetAngleYZTo(const C3DVector&,const C3DVector&);

#define FABLE_RETAIL_SHOVE_FRAME_PASS23
#include "engine/GFAngles.h"
#define FABLE_RETAIL_GFGETWORLD2DDISTANCE_X_FIRST
#include "engine/GFWorldDistance.h"
float __fastcall GFGetAngleYZTo(const C3DVector& pos1,const C3DVector& pos2)
{
    float adjacent = GFGetWorld2DDistance(pos1,pos2);
    float z1 = pos1.Z;
    float z2 = pos2.Z;
    return GET_ROUNDED_ANGLE(GFArcTan(z2-z1,adjacent));
}
void CThingBodyReorienter::ReorientBodyYZForSlopes() {
    if (!NGlobalConsole::UseBodyReorientationYZ) return;
    C3DVector head_pos;
    CRightHandedSet rhset;
    if (!Body->GetTCGraphicAppearance()->GetHelperPoint(HEAD_POS_HELPER_NAME,0,0,head_pos,1)) {
        if (!Body->GetTCGraphicAppearance()->GetDummyObject(EYE_LEFT_DUMMY_OBJECT_NAME,0,0,head_pos,rhset,1)) {
            if (!Body->GetTCGraphicAppearance()->GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,head_pos,1)) {
                FABLE_RETAIL_ASSERT_ARM(!"Body reorienter: should not beable to get here");
                return;
            }
        }
    }
    C3DVector foot_pos=Body->GetPos();
    C3DVector head_to_foot=head_pos-foot_pos;
    head_to_foot.Normalise();
    C3DVector up(0,0,1);
    float dot=head_to_foot.Dot(up);
    if (dot<MAX_DOT_FOR_ANGLE_YZ_CHANGE && dot>-MAX_DOT_FOR_ANGLE_YZ_CHANGE) {
        C3DVector above_head_pos=head_pos;
        above_head_pos.Z+=3.0f;
        float ground_head_height=CMainGameComponent::Get()->GetWorld().GetWorldMap().GetGroundSizeZAt(above_head_pos);
        C3DVector best_head_pos=head_pos-up*3;
        if (best_head_pos.Z<ground_head_height) best_head_pos.Z=ground_head_height;
        bool using_ground=true;
        CMeshIntersectionData data;
        if (false) { } else for (std::vector<CIntelligentPointer<CThing> >::iterator it=PossibleIntersectingThings.begin();it!=PossibleIntersectingThings.end();++it) {
            CThing* thing=it->Get();
            if (thing) {
                CPhysicsMeshObject physics_object=thing->GetPhysicsTC()->GetPhysicsMeshObject();
                if (physics_object.GetPointIntersectionFromTo(above_head_pos,best_head_pos,data)) {
                    const float land_tolerance_gap=0.007f;
                    float new_pos_z=data.IntersectionPosition.Z+land_tolerance_gap;
                    if (new_pos_z>best_head_pos.Z) { using_ground=false; best_head_pos.Z=new_pos_z; }
                }
            }
        }
        best_head_pos.Z+=0.12f;
        float best_angle_yz=GFGetAngleYZTo(foot_pos,best_head_pos);
        if (best_angle_yz>0.5f) best_angle_yz-=1.0f;
        float limit_angle=using_ground?0.17f:0.17f;
        if ((best_angle_yz>0 && best_angle_yz<limit_angle)||(best_angle_yz<0 && best_angle_yz>-limit_angle)) {
            C3DVector facing_vector=Body->GetPhysicsTC()->GetFacingVector();
            C3DVector foot_to_head=best_head_pos-foot_pos;
            float dot2=facing_vector.Dot(foot_to_head);
            if (dot2>0) Body->GetPhysicsTC()->SetFacingAngleYZ(best_angle_yz);
            else Body->GetPhysicsTC()->SetFacingAngleYZ(-best_angle_yz);
            if (Body->GetTypeFlags()&8) {
                CThingCreatureBase* creature=GFDebugDynamicCast(Body,(CThingCreatureBase*)0);
                float yz=creature->GetPhysicsTC()->GetFacingAngleYZ();
                if (fabs(yz)>DELTA_NOTIONAL_ZERO) creature->SetHadFacingAngleYZSetByBodyOrientation(true);
            }
        }
    }
}