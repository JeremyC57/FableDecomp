#ifndef FABLE_ENGINE_CENVIRONMENTSPELLEFFECTINTERFACE_H
#define FABLE_ENGINE_CENVIRONMENTSPELLEFFECTINTERFACE_H
// Retail opt-in (Codex pass18): original Ego_r3fd1c/3fd23/3fd2c,
// native timed-entry copy and both PDB nested-type spellings.
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
class CEnvironment;
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION {ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_INVALID=0,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE=1,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_SCREEN_SPACE=2};
class CEnvironmentSpellEffectInterface {
public:
 struct CRadialBlurParams {CRGBFloatColour ThresholdColour,IntensityColour;float Length,Fade,Range,Twist;};
 struct CRadialBlurPositionParams {C3DVector AreaCenter,BlurCenter;ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos,BlurTrackPos;};
 struct CDisplacementParams {CRGBFloatColour MonochromeEffectColour;};
 struct CScreenFilterParams {float Alpha,Saturation,Contrast,Brightness;CRGBFloatColour Tint;};
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
