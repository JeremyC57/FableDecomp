// 00b5c120 CEngineScreenEffectRadialBlurRenderer::UpdateWorldPositionTracking
// Leaf: ?UpdateWorldPositionTracking@CEngineScreenEffectRadialBlurRenderer@@AAE_NXZ
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_RADIAL_WORLD_TRACK_PASS29
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RENDER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_RADIAL_CAMERA_PROJECTION_PASS29
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
#define FABLE_RETAIL_RADIAL_MATRIX_SUM_PASS29
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CDisplayManager.h"
namespace NEngineInternal {
extern CEngineCamera* GPCamera;
extern CDisplayManager* GPDisplayManager;
}
using namespace NEngineInternal;
bool CEngineScreenEffectRadialBlurRenderer::UpdateWorldPositionTracking()
{
    if(BlurTrack3DPos) {
        C3DVector view_pos;
        GPCamera->TransformWorldToView(BlurCenterWorldTrackPosition,view_pos);
        if(view_pos.Z<GPCamera->PeekNearZ()) return false;
        C3DVector screen_pos;
        GPCamera->TransformViewToScreen(view_pos,screen_pos);
        BlurCenter.X=screen_pos.X/GPDisplayManager->GetRenderTargetDimensions().X;
        BlurCenter.Y=screen_pos.Y/GPDisplayManager->GetRenderTargetDimensions().Y;
    }
    if(AreaTrack3DPos) {
        C3DVector view_pos;
        GPCamera->TransformWorldToView(AreaCenterWorldTrackPosition,view_pos);
        if(view_pos.Z<GPCamera->PeekNearZ()) return false;
        C3DVector screen_pos;
        GPCamera->TransformViewToScreen(view_pos,screen_pos);
        AreaCenter.X=screen_pos.X/GPDisplayManager->GetRenderTargetDimensions().X;
        AreaCenter.Y=screen_pos.Y/GPDisplayManager->GetRenderTargetDimensions().Y;
    }
    return true;
}