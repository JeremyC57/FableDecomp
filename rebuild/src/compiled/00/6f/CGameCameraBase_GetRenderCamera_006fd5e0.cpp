// CGameCameraBase::GetRenderCamera(float) const -- interpolate camera position and look-at point for rendering
// Leaf: ?GetRenderCamera@CGameCameraBase@@UBE?AVCCamera@@M@Z
// Retail: 0x006FD5E0,212 bytes,virtual thiscall
#define FABLE_RETAIL_CGAMECAMERABASE_RENDER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_C3DVECTOR
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CGameCameraBase.h"
CCamera CGameCameraBase::GetRenderCamera(const float interpolate)const {
 CCamera passcam(InternalCam);
 C3DVector vecman;
 vecman.X=(GFGetInterpolatedValue(LastInternalCam.PeekWorldPos().X,InternalCam.PeekWorldPos().X,interpolate));
 vecman.Y=(GFGetInterpolatedValue(LastInternalCam.PeekWorldPos().Y,InternalCam.PeekWorldPos().Y,interpolate));
 vecman.Z=(GFGetInterpolatedValue(LastInternalCam.PeekWorldPos().Z,InternalCam.PeekWorldPos().Z,interpolate));
 passcam.MoveTo(vecman);
 vecman.X=(GFGetInterpolatedValue(LastPointingAt.X,PointingAt.X,interpolate));
 vecman.Y=(GFGetInterpolatedValue(LastPointingAt.Y,PointingAt.Y,interpolate));
 vecman.Z=(GFGetInterpolatedValue(LastPointingAt.Z,PointingAt.Z,interpolate));
 passcam.PointAt(vecman);
 return passcam;
}