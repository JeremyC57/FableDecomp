#include <stdio.h>
#include <string.h>
#include <stddef.h>
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

int main(){CEnvironmentSpellEffectInterface object;memset(&object,0,sizeof(object));CEnvironmentSpellEffectInterface::CScreenFilterParams value;memset(&value,0,sizeof(value));value.Alpha=0.5f;value.Saturation=1.0f;value.Contrast=1.5f;value.Brightness=2.0f;value.Tint.R=2.5f;value.Tint.G=3.0f;value.Tint.B=3.5f;value.Tint.A=4.0f;
 unsigned char before[sizeof(object)];memcpy(before,&object,sizeof(object));object.SetScreenFilterParams(value,true);
 int ok=memcmp(&object.ScreenFilterParams,&value,sizeof(value))==0 && memcmp(&object.PrevFrameScreenFilterParams,before+offsetof(CEnvironmentSpellEffectInterface,PrevFrameScreenFilterParams),sizeof(value))==0;
 size_t first=offsetof(CEnvironmentSpellEffectInterface,ScreenFilterParams),last=first+sizeof(value);
 for(size_t i=0;i<sizeof(object);++i)if(i<first || i>=last)ok=ok && ((unsigned char*)&object)[i]==before[i];
 object.SetScreenFilterParams(value,false);ok=ok && memcmp(&object.ScreenFilterParams,&value,sizeof(value))==0 && memcmp(&object.PrevFrameScreenFilterParams,&value,sizeof(value))==0;
 object.SetScreenFilterParams(object.PrevFrameScreenFilterParams,true);ok=ok && memcmp(&object.ScreenFilterParams,&value,sizeof(value))==0;
 object.SetScreenFilterParams(object.ScreenFilterParams,false);ok=ok && memcmp(&object.PrevFrameScreenFilterParams,&value,sizeof(value))==0;
 puts(ok?"00884fe0_TEST PASS":"00884fe0_TEST FAIL");return ok?0:1;
}