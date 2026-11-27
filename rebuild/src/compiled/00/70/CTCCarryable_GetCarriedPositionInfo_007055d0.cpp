// 007055d0 CTCCarryable::GetCarriedPositionInfo
// Leaf: ?GetCarriedPositionInfo@CTCCarryable@@QBE_NAAVC3DVector@@AAVCRightHandedSet@@_NM@Z
// Flags: /O2 /Oy /Gr
// Bracket: GetRight uses its shared X grouping. The Z addition groups the final component.
// Inline-budget: retail expands TransformRightHandedSet between PostMultiply and the second pickup-offset query.
#define FABLE_RETAIL_CARRYABLE_POSE_PASS29
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CTCCARRYABLE_ISCARRIED
#define FABLE_RETAIL_CTCCARRYING_INTERPOLATEDPOS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_C3DVECTOR_CROSS_X_GROUP
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/C3DMeshObject.h"
bool CTCCarryable::GetCarriedPositionInfo(C3DVector& position,CRightHandedSet& rhset,bool force_accurate,float interpolate) const
{
    if (!PCarryingTC || !IsCarried()) return false;
    CRightHandedSet carrying_dummy_rhs;
    C3DVector carrying_dummy_pos;
    if (!PCarryingTC->GetCarriedThingInterpolatedPosAndOrientation(GetParentThing(),carrying_dummy_pos,carrying_dummy_rhs,force_accurate,interpolate)) {
        return false;
    } else {
        C3DVector offset_x=carrying_dummy_rhs.GetForward();
        C3DVector offset_y=carrying_dummy_rhs.GetRight();
        C3DVector offset_z=carrying_dummy_rhs.GetUp();
        C3DVector offset_world_coord=(offset_x*OffsetCoordRelativeToAttachToDummy.X+offset_y*OffsetCoordRelativeToAttachToDummy.Y+offset_z*OffsetCoordRelativeToAttachToDummy.Z)*OffsetAmount;
        CRightHandedSet object_dummy_rhs;
        C3DVector object_dummy_offset;
        GetInterpolatedPickupOffsetAndOrientation(object_dummy_offset,object_dummy_rhs,force_accurate,interpolate);
        CRightHandedSet object_rhs=GetParentThing().GetPhysicsTC()->GetRHSet(interpolate);
        CMatrix3x4 obj_dummy_2_obj=C3DMeshObject::GetRotationMatrixForTransformFromTo(object_dummy_rhs,object_rhs);
        CMatrix3x4 obj_2_carry_dummy=C3DMeshObject::GetRotationMatrixForTransformFromTo(object_rhs,carrying_dummy_rhs);
        CMatrix3x4 mat=obj_dummy_2_obj;
        mat.PostMultiply(obj_2_carry_dummy);
        mat.TransformRightHandedSet(object_rhs);
        GetInterpolatedPickupOffsetAndOrientationAt(GetParentThing().GetPos(),object_rhs,object_dummy_offset,object_dummy_rhs,force_accurate);
        rhset=object_rhs;
        position=carrying_dummy_pos-object_dummy_offset+offset_world_coord;
        return true;
    }
}