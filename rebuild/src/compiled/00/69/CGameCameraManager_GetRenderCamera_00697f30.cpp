// CGameCameraManager::GetRenderCamera(float interpolation) const -- the camera to render: GetInterpolatedCamera(interpolation, Camera, OldCamera)
// Leaf: ?GetRenderCamera@CGameCameraManager@@UBE?AVCCamera@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697F30 (CAIGameCameraBase slot 1).
// NF28 (2026-11-12, queue land1011ei): was landed through the stand-in CGameCameraManagerMethods calling Helper6978d0 with local
//   CCamera / CameraState models. The callee 0x006978D0 is ?GetInterpolatedCamera@CGameCameraManager@@ABE?AVCCamera@@MABV2@0@Z (Ego_r.pdb
//   0x006BE4E0 / FableWin.pdb 0x02202AE0, private const; its only caller is this function at 0x00697F43); this override is Ego_r.pdb
//   0x006BECB0 ?GetRenderCamera@CGameCameraManager@@UBE?AVCCamera@@M@Z. Model: engine/CGameCameraManager.h (Camera / OldCamera) with
//   the GETRENDERCAMERA declaration opt-in and the defines of the landed 0x006978D0 row.
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_INTERPOLATEDCAMERA
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCAMERA_SETVIEW
#define FABLE_RETAIL_CCAMERA_UP_DIRECTION
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETRENDERCAMERA
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CGameCameraManager.h"

CCamera CGameCameraManager::GetRenderCamera(float interpolation) const
{
    return GetInterpolatedCamera(interpolation, Camera, OldCamera);
}
