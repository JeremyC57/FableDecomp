// CEngineScreenEffectRadialBlurRenderer::CaptureEnvironmentParameters() -- copy the current environment's radial-blur settings into CurrentParameters and the blur/area centres (3D track positions when the environment tracks them, else the 2D screen centres)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Leaf: ?CaptureEnvironmentParameters@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BD70, length 262 bytes.
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C (returns the current
// environment), virtual slot 1 of that environment (returns its settings record; the radial-blur block sits at +0x60).
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h retail branch, MEMBERS (CurrentParameters +0x198.., centres +0x170.., track
// flags +0x1c0/+0x1c1). The environment's radial-blur block is modelled locally: blur centre C3DVector +0, area centre
// C3DVector +0x0c, thresholds +0x18, intensities +0x28, length/fade/range/twist +0x38.., track modes +0x48/+0x4c (longs).
// Shape: the parameters are copied member by member in retail's store order (fade, range, length, twist, intensities,
// thresholds); each track flag is `mode == 1`; the 3D positions are whole C3DVector assignments (both addresses formed
// with `lea`), the 2D centres take X and Y separately.
// Naming (NF177, PRIORITY q12085): the row-local CEngineScreenEffectRadialBlurRenderer_Env stand-in (QAE, over the generated flat layout)
//   replaced by the header's retail branch (FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER _MEMBERS / _RENDER, as the caller 0x00B5EA30):
//   Ego_r.pdb + FableWin.pdb private ?CaptureEnvironmentParameters@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ, the spelling 0x00B5EA30 binds.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RENDER
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"


#include "engine/C3DVector.h"

// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A); the "environment provider at +0x198" is its GameWorld (CIDrawWorld*,
// FableWin.pdb inline PeekGameWorld, engine/CEngineWorldMap.h _GAMEWORLD), slot 1 is CIDrawWorld::DrawGetEnvironment and the environment's
// slot 1 CIDrawEnvironment::PeekInterpolatedParams (engine/CIDrawWorld.h / CIDrawEnvironment.h); the radial-blur block (+0x60) is
// CIDrawEnvironmentParams::ScreenEffectParams.RadialBlur (CIDrawEnvironmentRadialBlurParameters, Ego_r.pdb members). Were the local
// SEngineState_01436e8c / g_pEngineState_01436e8c, CEnvironmentProvider, CEnvironment, SEnvironmentSettings and SEnvironmentRadialBlur.
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

void CEngineScreenEffectRadialBlurRenderer::CaptureEnvironmentParameters()
{
    const CIDrawEnvironmentRadialBlurParameters& env = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ScreenEffectParams.RadialBlur;
    CurrentParameters.Fade = env.Fade;
    CurrentParameters.Range = env.Range;
    CurrentParameters.Length = env.Length;
    CurrentParameters.Twist = env.Twist;
    CurrentParameters.IntensityR = env.IntensityColour.R;
    CurrentParameters.IntensityG = env.IntensityColour.G;
    CurrentParameters.IntensityB = env.IntensityColour.B;
    CurrentParameters.ThresholdR = env.ThresholdColour.R;
    CurrentParameters.ThresholdG = env.ThresholdColour.G;
    CurrentParameters.ThresholdB = env.ThresholdColour.B;
    AreaTrack3DPos = env.AreaTrackPos == ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE;
    BlurTrack3DPos = env.BlurTrackPos == ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE;
    if (AreaTrack3DPos) {
        AreaCenterWorldTrackPosition = env.AreaCenter;
    } else {
        AreaCenter.X = env.AreaCenter.X;
        AreaCenter.Y = env.AreaCenter.Y;
    }
    if (BlurTrack3DPos) {
        BlurCenterWorldTrackPosition = env.BlurCenter;
    } else {
        BlurCenter.X = env.BlurCenter.X;
        BlurCenter.Y = env.BlurCenter.Y;
    }
}
