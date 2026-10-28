#include <stdio.h>
#include <string.h>
#include <stddef.h>
// CEnvironmentSpellEffectInterface::SetRadialBlurPositionParams(const Params& value, bool interpolate) -- Set current effect parameters and optionally reset the previous frame.
// Leaf: ?SetRadialBlurPositionParams@CEnvironmentSpellEffectInterface@@QAEXABUCRadialBlurPositionParams@1@_N@Z
// Identity: original PDB nested struct and setter name; exact field offsets52/180 identify this overload.
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
 void SetRadialBlurPositionParams(const CRadialBlurPositionParams&,bool);
};
void CEnvironmentSpellEffectInterface::SetRadialBlurPositionParams(const CRadialBlurPositionParams& value,bool interpolate){RadialBlurPositionParams=value;if(!interpolate)PrevFrameRadialBlurPositionParams=value;}

int main(){CEnvironmentSpellEffectInterface object;memset(&object,0,sizeof(object));CEnvironmentSpellEffectInterface::CRadialBlurPositionParams value;memset(&value,0,sizeof(value));value.AreaCenter.X=0.5f;value.AreaCenter.Y=1.0f;value.AreaCenter.Z=1.5f;value.BlurCenter.X=2.0f;value.BlurCenter.Y=2.5f;value.BlurCenter.Z=3.0f;value.AreaTrackPos=(ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION)1;value.BlurTrackPos=(ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION)2;
 unsigned char before[sizeof(object)];memcpy(before,&object,sizeof(object));object.SetRadialBlurPositionParams(value,true);
 int ok=memcmp(&object.RadialBlurPositionParams,&value,sizeof(value))==0 && memcmp(&object.PrevFrameRadialBlurPositionParams,before+offsetof(CEnvironmentSpellEffectInterface,PrevFrameRadialBlurPositionParams),sizeof(value))==0;
 size_t first=offsetof(CEnvironmentSpellEffectInterface,RadialBlurPositionParams),last=first+sizeof(value);
 for(size_t i=0;i<sizeof(object);++i)if(i<first || i>=last)ok=ok && ((unsigned char*)&object)[i]==before[i];
 object.SetRadialBlurPositionParams(value,false);ok=ok && memcmp(&object.RadialBlurPositionParams,&value,sizeof(value))==0 && memcmp(&object.PrevFrameRadialBlurPositionParams,&value,sizeof(value))==0;
 object.SetRadialBlurPositionParams(object.PrevFrameRadialBlurPositionParams,true);ok=ok && memcmp(&object.RadialBlurPositionParams,&value,sizeof(value))==0;
 object.SetRadialBlurPositionParams(object.RadialBlurPositionParams,false);ok=ok && memcmp(&object.PrevFrameRadialBlurPositionParams,&value,sizeof(value))==0;
 puts(ok?"00884f60_TEST PASS":"00884f60_TEST FAIL");return ok?0:1;
}