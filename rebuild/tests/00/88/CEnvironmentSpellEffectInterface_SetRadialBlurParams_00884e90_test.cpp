#include <stdio.h>
#include <string.h>
#include <stddef.h>
// CEnvironmentSpellEffectInterface::SetRadialBlurParams(const Params& value, bool interpolate) -- Set current effect parameters and optionally reset the previous frame.
// Leaf: ?SetRadialBlurParams@CEnvironmentSpellEffectInterface@@QAEXABUCRadialBlurParams@1@_N@Z
// Identity: original PDB nested struct and setter name; exact field offsets4/132 identify this overload.
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
 void SetRadialBlurParams(const CRadialBlurParams&,bool);
};
void CEnvironmentSpellEffectInterface::SetRadialBlurParams(const CRadialBlurParams& value,bool interpolate){RadialBlurParams=value;if(!interpolate)PrevFrameRadialBlurParams=value;}

int main(){CEnvironmentSpellEffectInterface object;memset(&object,0,sizeof(object));CEnvironmentSpellEffectInterface::CRadialBlurParams value;memset(&value,0,sizeof(value));value.ThresholdColour.R=0.5f;value.ThresholdColour.G=1.0f;value.ThresholdColour.B=1.5f;value.ThresholdColour.A=2.0f;value.IntensityColour.R=2.5f;value.IntensityColour.G=3.0f;value.IntensityColour.B=3.5f;value.IntensityColour.A=4.0f;value.Length=4.5f;value.Fade=5.0f;value.Range=5.5f;value.Twist=6.0f;
 unsigned char before[sizeof(object)];memcpy(before,&object,sizeof(object));object.SetRadialBlurParams(value,true);
 int ok=memcmp(&object.RadialBlurParams,&value,sizeof(value))==0 && memcmp(&object.PrevFrameRadialBlurParams,before+offsetof(CEnvironmentSpellEffectInterface,PrevFrameRadialBlurParams),sizeof(value))==0;
 size_t first=offsetof(CEnvironmentSpellEffectInterface,RadialBlurParams),last=first+sizeof(value);
 for(size_t i=0;i<sizeof(object);++i)if(i<first || i>=last)ok=ok && ((unsigned char*)&object)[i]==before[i];
 object.SetRadialBlurParams(value,false);ok=ok && memcmp(&object.RadialBlurParams,&value,sizeof(value))==0 && memcmp(&object.PrevFrameRadialBlurParams,&value,sizeof(value))==0;
 object.SetRadialBlurParams(object.PrevFrameRadialBlurParams,true);ok=ok && memcmp(&object.RadialBlurParams,&value,sizeof(value))==0;
 object.SetRadialBlurParams(object.RadialBlurParams,false);ok=ok && memcmp(&object.PrevFrameRadialBlurParams,&value,sizeof(value))==0;
 puts(ok?"00884e90_TEST PASS":"00884e90_TEST FAIL");return ok?0:1;
}