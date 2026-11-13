#ifndef FABLE_ENGINE_CENVIRONMENTSPELLEFFECTINTERFACE_H
#define FABLE_ENGINE_CENVIRONMENTSPELLEFFECTINTERFACE_H
// Retail opt-in (Codex pass18): original Ego_r3fd1c/3fd23/3fd2c,
// native timed-entry copy and both PDB nested-type spellings.
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
class CEnvironment;
#ifndef FABLE_ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_DEFINED
#define FABLE_ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_DEFINED
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION {ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_INVALID=0,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE=1,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_SCREEN_SPACE=2};
#endif
#ifdef FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_RENDERFRAME
#include "engine/CIDrawEnvironmentScreenEffectParams.h"
#endif
class CEnvironmentSpellEffectInterface {
public:
 struct CRadialBlurParams {CRGBFloatColour ThresholdColour,IntensityColour;float Length,Fade,Range,Twist;};
 struct CRadialBlurPositionParams {C3DVector AreaCenter,BlurCenter;ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos,BlurTrackPos;};
 struct CDisplacementParams {CRGBFloatColour MonochromeEffectColour;};
 struct CScreenFilterParams {float Alpha,Saturation,Contrast,Brightness;CRGBFloatColour Tint;};
#ifdef FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_RENDERFRAME
 // Retail opt-in (chunk 2019, row 0x006B42F0 CEnvironment::RenderUpdate): Ego_r.pdb names
 // ?GetRenderFrameInterpolatedRadialBlurParameters@CEnvironmentSpellEffectInterface@@QAE?AUCIDrawEnvironmentRadialBlurParameters@@M@Z,
 // ?GetRenderFrameInterpolatedDisplacementParameters@..@QAE?AUCIDrawEnvironmentDisplacementParameters@@M@Z and
 // ?GetRenderFrameInterpolatedScreenFilterParameters@..@QAE?AUCIDrawEnvironmentScreenFilterParameters@@M@Z, out of line in retail at
 // 0x008857E0 / 0x00885900 / 0x008859F0 (RenderUpdate calls them in that order on SpellEffectInterface +0x1960 with the hidden result
 // pointer and the frame fraction, 0x006B483C / 0x006B485C / 0x006B4889). Declarations only.
 CIDrawEnvironmentRadialBlurParameters GetRenderFrameInterpolatedRadialBlurParameters(float frame_fraction);       // 0x008857E0
 CIDrawEnvironmentDisplacementParameters GetRenderFrameInterpolatedDisplacementParameters(float frame_fraction);   // 0x00885900
 CIDrawEnvironmentScreenFilterParameters GetRenderFrameInterpolatedScreenFilterParameters(float frame_fraction);   // 0x008859F0
#endif
private:
 CEnvironment* Environment;
 CRadialBlurParams RadialBlurParams;
 CRadialBlurPositionParams RadialBlurPositionParams;
 CDisplacementParams DisplacementParams;
 CScreenFilterParams ScreenFilterParams;
 CRadialBlurParams PrevFrameRadialBlurParams;
 CRadialBlurPositionParams PrevFrameRadialBlurPositionParams;
 CDisplacementParams PrevFrameDisplacementParams;
 CScreenFilterParams PrevFrameScreenFilterParams;
};
#endif
