// CGameCameraManager::UpdateRightStickLookAround -- update the camera look-around direction from the right stick.
// Retail: 0x00697CC0, 346 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?UpdateRightStickLookAround@CGameCameraManager@@AAEXXZ
// Flags: none
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCAMERA_SETVIEW
#define FABLE_RETAIL_CCAMERA_UP_DIRECTION
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKAROUND
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CBase.h"
void CGameCameraManager::UpdateRightStickLookAround()
{
 RightStickLookAroundAngles=GetCameraTools().GetRollingAverage(RightStickLookAroundAngles,RightStickTargetLookAroundAngles,3.0f);
 if(GetCameraMode()->IsUseRightStickLookAround()){
  C3DVector forward=Camera.PeekViewDirection();
  C3DVector up=Camera.PeekUpDirection();
  C3DVector right=forward.Cross(up);
  float rotation_angle_x=RightStickLookAroundAngles.X;
  forward.Rotate(up,rotation_angle_x);
  right.Rotate(up,rotation_angle_x);
  float rotation_angle_y=RightStickLookAroundAngles.Y;
  forward.Rotate(right,rotation_angle_y);
  up.Rotate(right,rotation_angle_y);
  forward.AccurateNormalise();
  up.AccurateNormalise();
  Camera.SetView(forward,up);
 }
}