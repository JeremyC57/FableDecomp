// CEnvironmentSpellEffectInterface::SetScreenFilterParams(const Params& value, bool interpolate) -- Set current effect parameters and optionally reset the previous frame.
// Leaf: ?SetScreenFilterParams@CEnvironmentSpellEffectInterface@@QAEXABUCScreenFilterParams@1@_N@Z
// Identity: original PDB nested struct and setter name; exact field offsets100/228 identify this overload.
class CEnvironment;
class CRGBFloatColour {public:float R,G,B,A;};
#pragma pack(push,1)
class C3DVector {public:float X,Y,Z;};
#pragma pack(pop)
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION {DescriptiveTrackZero=0};
// Layout: no active shared owner/parameter headers. PDB named fields agree with retail copies: current+4/+52/+100, previous+132/+180/+228; sizes48/32/32.
class CEnvironmentSpellEffectInterface {public:
 struct CRadialBlurParams {CRGBFloatColour ThresholdColour,IntensityColour;float Length,Fade,Range,Twist;};
 struct CRadialBlurPositionParams {C3DVector AreaCenter,BlurCenter;ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos,BlurTrackPos;};
 struct CDisplacementParams {CRGBFloatColour MonochromeEffectColour;};
 struct CScreenFilterParams {float Alpha,Saturation,Contrast,Brightness;CRGBFloatColour Tint;};
 CEnvironment* Environment;CRadialBlurParams RadialBlurParams;CRadialBlurPositionParams RadialBlurPositionParams;CDisplacementParams DisplacementParams;CScreenFilterParams ScreenFilterParams;
 CRadialBlurParams PrevFrameRadialBlurParams;CRadialBlurPositionParams PrevFrameRadialBlurPositionParams;CDisplacementParams PrevFrameDisplacementParams;CScreenFilterParams PrevFrameScreenFilterParams;
 void SetScreenFilterParams(const CScreenFilterParams&,bool);
};
void CEnvironmentSpellEffectInterface::SetScreenFilterParams(const CScreenFilterParams& value,bool interpolate){ScreenFilterParams=value;if(!interpolate)PrevFrameScreenFilterParams=value;}