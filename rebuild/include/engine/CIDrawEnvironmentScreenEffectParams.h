#ifndef FABLE_ENGINE_CIDRAWENVIRONMENTSCREENEFFECTPARAMS_H
#define FABLE_ENGINE_CIDRAWENVIRONMENTSCREENEFFECTPARAMS_H
// New header (chunk 2019, row 0x006B42F0 CEnvironment::RenderUpdate). The environment's screen-effect parameter structs, Ego_r.pdb
// layouts (struct_layouts_egor.tsv; no container member, so Ego_r = retail) and keys (pdb_class_keys.tsv: U for the three
// CIDrawEnvironment*Parameters, all returned by value from CEnvironmentSpellEffectInterface::GetRenderFrameInterpolated*Parameters):
//   CIDrawEnvironmentRadialBlurParameters    0x50: BlurCenter, AreaCenter, ThresholdColour, IntensityColour, Length, Fade, Range,
//                                                  Twist, BlurTrackPos, AreaTrackPos (RenderUpdate copies it with `rep movsd` x 0x14)
//   CIDrawEnvironmentDisplacementParameters 0x10: MonochromeEffectColour
//   CIDrawEnvironmentScreenEffectParams     0x60: RadialBlur +0x00, Displacement +0x50 (CIDrawEnvironmentParams +0x60 / +0xB0 in retail:
//                                                  RenderUpdate stores the two at this +0x18F4 / +0x1944)
//   CIDrawEnvironmentScreenFilterParameters 0x20: Alpha, Saturation, Contrast, Brightness, Tint
// The special members the PDBs list (??0CIDrawEnvironmentScreenEffectParams@@QAE@XZ) are not declared: nothing here constructs one.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#ifndef FABLE_ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_DEFINED
#define FABLE_ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_DEFINED
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION {ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_INVALID=0,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE=1,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_SCREEN_SPACE=2};
#endif
struct CIDrawEnvironmentRadialBlurParameters {
    C3DVector                               BlurCenter;          // +0x00
    C3DVector                               AreaCenter;          // +0x0c
    CRGBFloatColour                         ThresholdColour;     // +0x18
    CRGBFloatColour                         IntensityColour;     // +0x28
    float                                   Length;              // +0x38
    float                                   Fade;                // +0x3c
    float                                   Range;               // +0x40
    float                                   Twist;               // +0x44
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION  BlurTrackPos;        // +0x48
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION  AreaTrackPos;        // +0x4c
};
struct CIDrawEnvironmentDisplacementParameters {
    CRGBFloatColour                         MonochromeEffectColour; // +0x00
};
struct CIDrawEnvironmentScreenEffectParams {
    CIDrawEnvironmentRadialBlurParameters   RadialBlur;          // +0x00
    CIDrawEnvironmentDisplacementParameters Displacement;        // +0x50
};
struct CIDrawEnvironmentScreenFilterParameters {
    float                                   Alpha;               // +0x00
    float                                   Saturation;          // +0x04
    float                                   Contrast;            // +0x08
    float                                   Brightness;          // +0x0c
    CRGBFloatColour                         Tint;                // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentRadialBlurParameters) == 0x50);
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentDisplacementParameters) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentScreenEffectParams) == 0x60);
FABLE_STATIC_ASSERT(sizeof(CIDrawEnvironmentScreenFilterParameters) == 0x20);
#endif // FABLE_ENGINE_CIDRAWENVIRONMENTSCREENEFFECTPARAMS_H
