// CEngineScreenEffectRadialBlurRenderer::ResetParameters() -- restores radial blur defaults.
// Retail00b5bbc0 length117; manifest ShadowRenderer label is propagated incorrectly.
// Sole caller00b5ca52 in radial-blur constructor; actual header fields match all stores.
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
struct CRadialBlurMethods : CEngineScreenEffectRadialBlurRenderer { void ResetParameters(); };
void CRadialBlurMethods::ResetParameters() {
 CurrentParameters_ThresholdR=0.5f; CurrentParameters_ThresholdG=0.5f; CurrentParameters_ThresholdB=0.5f;
 CurrentParameters_IntensityR=0.8f; CurrentParameters_IntensityG=0.8f; CurrentParameters_IntensityB=0.8f;
 CurrentParameters_Length=6.0f;
 CurrentParameters_Range=0.0f; CurrentParameters_Twist=1.0f; CurrentParameters_Fade=0.0f;
 BlurCenter_X=0.5f; BlurCenter_Y=0.5f; AreaCenter_X=0.5f; AreaCenter_Y=0.5f;
 AreaTrack3DPos=false; BlurTrack3DPos=false;
}