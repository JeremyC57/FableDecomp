// CCamera::CCamera -- build an orthogonal camera basis toward a target.
// Flags:
// Retail: 00a0bf20,517 bytes,thiscall constructor with two vectors,float,bool.
// Leaf: ??0CCamera@@QAE@ABVC3DVector@@0M_N@Z
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_CCAMERA_TARGET_CTOR
#include "engine/CCamera.h"
CCamera::CCamera(const C3DVector& world_position,const C3DVector& target_position,float fov,bool is_2d)
{
 C3DVector temp_up(0.0f,0.0f,1.0f);
 C3DVector up_difference;
 WorldPosition=world_position;
 C3DVector view=target_position-WorldPosition;
 view.Normalise();
 float factor=(temp_up.Dot(view));
 up_difference=view*factor;
 C3DVector up=temp_up-up_difference;
 up.Normalise();
 RHSet.Set(up,view);
 HorizontalFOV=fov;VerticalFOV=0.0f;Zoom=fov;
 FOVFlags=is_2d?4:1;
 HeightLocked=false;ViewVecZLocked=false;
}