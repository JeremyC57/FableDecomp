// CEngineScreenEffectRadialBlurRenderer::SetDefaultParams() -- restore the radial blur defaults: thresholds 0.5, intensities 0.8,
//   length 6, range 0, twist 1, fade 0, both centres (0.5, 0.5), no 3D tracking
// Leaf: ?SetDefaultParams@CEngineScreenEffectRadialBlurRenderer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5BBC0, 117 bytes; sole caller the constructor 0x00B5C460 (call 0x00B5CA52).
// Identity (NF59, round 7, queue q10551): Ego_r.pdb 0x00A0DD80 / FableWin.pdb 0x02D1C070 ?SetDefaultParams@CEngineScreenEffectRadialBlurRenderer@@QAEXXZ;
//   the body stores the CurrentParameters defaults (+0x198..+0x1BC) and the centres; Ego_r order SetDefaultParams before
//   InitialiseTextures = retail 0x00B5BBC0 before 0x00B5BF10; the landed ctor 0x00B5C460 binds this spelling (NF57). Re-authored on
//   engine/CEngineScreenEffectRadialBlurRenderer.h's retail _MEMBERS branch (opt-in _SETDEFAULTPARAMS, the 0x00B5C460 include block):
//   the landed row defined the made-up ResetParameters on a local wrapper struct CRadialBlurMethods (catalog SetShadowClipVolumesToNeverShadow
//   is a propagated manifest label).
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_SETDEFAULTPARAMS
#include "engine/CEngineInit.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

void CEngineScreenEffectRadialBlurRenderer::SetDefaultParams()
{
    CurrentParameters.ThresholdR = 0.5f;
    CurrentParameters.ThresholdG = 0.5f;
    CurrentParameters.ThresholdB = 0.5f;
    CurrentParameters.IntensityR = 0.8f;
    CurrentParameters.IntensityG = 0.8f;
    CurrentParameters.IntensityB = 0.8f;
    CurrentParameters.Length = 6.0f;
    CurrentParameters.Range = 0.0f;
    CurrentParameters.Twist = 1.0f;
    CurrentParameters.Fade = 0.0f;
    BlurCenter.X = 0.5f;
    BlurCenter.Y = 0.5f;
    AreaCenter.X = 0.5f;
    AreaCenter.Y = 0.5f;
    AreaTrack3DPos = false;
    BlurTrack3DPos = false;
}
