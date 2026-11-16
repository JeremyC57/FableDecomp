// CEngineScreenEffectRadialBlurRenderer::CaptureEnvironmentParameters() -- copy the current environment's radial-blur settings into CurrentParameters and the blur/area centres (3D track positions when the environment tracks them, else the 2D screen centres)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BD70, length 262 bytes.
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C (returns the current
// environment), virtual slot 1 of that environment (returns its settings record; the radial-blur block sits at +0x60).
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h (CurrentParameters +0x198.., centres +0x170.., track flags
// +0x1c0/+0x1c1). The environment's radial-blur block is modelled locally: blur centre C3DVector +0, area centre
// C3DVector +0x0c, thresholds +0x18, intensities +0x28, length/fade/range/twist +0x38.., track modes +0x48/+0x4c (longs).
// Shape: the parameters are copied member by member in retail's store order (fade, range, length, twist, intensities,
// thresholds); each track flag is `mode == 1`; the 3D positions are whole C3DVector assignments (both addresses formed
// with `lea`), the 2D centres take X and Y separately.
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

struct CEngineScreenEffectRadialBlurRenderer_Env : CEngineScreenEffectRadialBlurRenderer {
    C3DVector& BlurTrackPosition() { return *reinterpret_cast<C3DVector*>(&BlurCenterWorldTrackPosition_X); }
    C3DVector& AreaTrackPosition() { return *reinterpret_cast<C3DVector*>(&AreaCenterWorldTrackPosition_X); }
    void CaptureEnvironmentParameters();
};

void CEngineScreenEffectRadialBlurRenderer_Env::CaptureEnvironmentParameters()
{
    const CIDrawEnvironmentRadialBlurParameters& env = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ScreenEffectParams.RadialBlur;
    CurrentParameters_Fade = env.Fade;
    CurrentParameters_Range = env.Range;
    CurrentParameters_Length = env.Length;
    CurrentParameters_Twist = env.Twist;
    CurrentParameters_IntensityR = env.IntensityColour.R;
    CurrentParameters_IntensityG = env.IntensityColour.G;
    CurrentParameters_IntensityB = env.IntensityColour.B;
    CurrentParameters_ThresholdR = env.ThresholdColour.R;
    CurrentParameters_ThresholdG = env.ThresholdColour.G;
    CurrentParameters_ThresholdB = env.ThresholdColour.B;
    AreaTrack3DPos = env.AreaTrackPos == ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE;
    BlurTrack3DPos = env.BlurTrackPos == ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE;
    if (AreaTrack3DPos) {
        AreaTrackPosition() = env.AreaCenter;
    } else {
        AreaCenter_X = env.AreaCenter.X;
        AreaCenter_Y = env.AreaCenter.Y;
    }
    if (BlurTrack3DPos) {
        BlurTrackPosition() = env.BlurCenter;
    } else {
        BlurCenter_X = env.BlurCenter.X;
        BlurCenter_Y = env.BlurCenter.Y;
    }
}