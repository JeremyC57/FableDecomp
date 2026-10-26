#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
// Actual GetRenderFrameInterpolatedRadialBlurParameters(float), not CFaction.
// Native non-POD aggregate returns put the hidden result reference in ECX,
// previous reference in EDX and fraction/current on stack, returning result in EAX.
// Default constructors are declared only; no runtime initialization is invented.
// PDB egor22495; native helpers8854A0/885680 interpolate params/positions.
// No actual headers for these four types. PDB layouts agree exactly:
// interface current4/34, previous84/B4; params48, positions32, output80.
#include <stddef.h>
struct CBlurVector {float X,Y,Z;};
struct CBlurColour {float R,G,B,A;};
struct CBlurParams {CBlurParams();CBlurColour ThresholdColour,IntensityColour;float Length,Fade,Range,Twist;};
struct CBlurPositionParams {CBlurPositionParams();CBlurVector AreaCenter,BlurCenter;int AreaTrackPos,BlurTrackPos;};
struct CIDrawEnvironmentRadialBlurParameters {
 CIDrawEnvironmentRadialBlurParameters(){} //fablewin66178 real default ctor
 CIDrawEnvironmentRadialBlurParameters(const CIDrawEnvironmentRadialBlurParameters&); //885A80 actual80-byte copying constructor
 CBlurVector BlurCenter,AreaCenter;CBlurColour ThresholdColour,IntensityColour;
 float Length,Fade,Range,Twist;int BlurTrackPos,AreaTrackPos;
};
class CEnvironment;
class CEnvironmentSpellEffectInterface { public:
 CEnvironmentSpellEffectInterface(CEnvironment*); //native8852E0; fablewin94147
 CEnvironment* Environment;CBlurParams RadialBlurParams;CBlurPositionParams RadialBlurPositionParams;
 unsigned char OtherCurrent[48];CBlurParams PrevFrameRadialBlurParams;CBlurPositionParams PrevFrameRadialBlurPositionParams;unsigned char OtherPrevious[48];
 static CBlurParams __fastcall InterpolateRadialBlurParams(float,const CBlurParams&,const CBlurParams&);
 static CBlurPositionParams __fastcall InterpolateRadialBlurPositionParams(float,const CBlurPositionParams&,const CBlurPositionParams&);
 CIDrawEnvironmentRadialBlurParameters GetRenderFrameInterpolatedRadialBlurParameters(float);
};
#ifdef _MSC_VER
typedef char Params48[sizeof(CBlurParams)==48?1:-1];
typedef char Position32[sizeof(CBlurPositionParams)==32?1:-1];
typedef char Draw80[sizeof(CIDrawEnvironmentRadialBlurParameters)==80?1:-1];
typedef char NativeOwner104[sizeof(CEnvironmentSpellEffectInterface)==0x104?1:-1];
typedef char Previous84[offsetof(CEnvironmentSpellEffectInterface,PrevFrameRadialBlurParams)==0x84?1:-1];
#endif
CIDrawEnvironmentRadialBlurParameters CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedRadialBlurParameters(float fraction){
 CBlurParams p=InterpolateRadialBlurParams(fraction,PrevFrameRadialBlurParams,RadialBlurParams);
 CBlurPositionParams pos=InterpolateRadialBlurPositionParams(fraction,PrevFrameRadialBlurPositionParams,RadialBlurPositionParams);
 CIDrawEnvironmentRadialBlurParameters result;
 result.Fade=p.Fade;result.Length=p.Length;result.Range=p.Range;result.Twist=p.Twist;
 result.IntensityColour=p.IntensityColour;result.ThresholdColour=p.ThresholdColour;
 result.AreaCenter=pos.AreaCenter;result.BlurCenter=pos.BlurCenter;
 result.AreaTrackPos=pos.AreaTrackPos;result.BlurTrackPos=pos.BlurTrackPos;
 return result;
}



#include <assert.h>
#include <stdio.h>
#include <string.h>
// Genuine default ctor declarations: fablewin PDB67079/67083. Native owner
//8852E0 inlines zero params and tracking fields, leaves vectors untouched.
CBlurParams::CBlurParams(){ThresholdColour.R=ThresholdColour.G=ThresholdColour.B=ThresholdColour.A=0;IntensityColour=ThresholdColour;Length=Fade=Range=Twist=0;}
CBlurPositionParams::CBlurPositionParams():AreaTrackPos(0),BlurTrackPos(0){}
CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface(CEnvironment* p):Environment(p){
 PrevFrameRadialBlurParams=RadialBlurParams;
 // Native uses rep movsd, preserving uninitialized position-vector bytes.
 memcpy(&PrevFrameRadialBlurPositionParams,&RadialBlurPositionParams,sizeof(RadialBlurPositionParams));
 const float defaults[12]={0,0,0,0,0,1,0,0,1,1,1,1};
 memcpy(OtherCurrent,defaults,sizeof(defaults));memcpy(OtherPrevious,defaults,sizeof(defaults));
}
CIDrawEnvironmentRadialBlurParameters::CIDrawEnvironmentRadialBlurParameters(const CIDrawEnvironmentRadialBlurParameters& other){*this=other;}
static CEnvironmentSpellEffectInterface* owner;
static CBlurParams expectedParams;
static CBlurPositionParams expectedPosition;
static float expectedFraction;
static int calls;
static bool SameFloat(float a,float b){return memcmp(&a,&b,sizeof(a))==0;}
CBlurParams __fastcall CEnvironmentSpellEffectInterface::InterpolateRadialBlurParams(float fraction,const CBlurParams& previous,const CBlurParams& current){
 assert(calls++==0&&&previous==&owner->PrevFrameRadialBlurParams&&&current==&owner->RadialBlurParams&&SameFloat(fraction,expectedFraction));return expectedParams;
}
CBlurPositionParams __fastcall CEnvironmentSpellEffectInterface::InterpolateRadialBlurPositionParams(float fraction,const CBlurPositionParams& previous,const CBlurPositionParams& current){
 assert(calls++==1&&&previous==&owner->PrevFrameRadialBlurPositionParams&&&current==&owner->RadialBlurPositionParams&&SameFloat(fraction,expectedFraction));return expectedPosition;
}
static void CheckColour(const CBlurColour& a,const CBlurColour& b){assert(SameFloat(a.R,b.R)&&SameFloat(a.G,b.G)&&SameFloat(a.B,b.B)&&SameFloat(a.A,b.A));}
static void CheckVector(const CBlurVector& a,const CBlurVector& b){assert(SameFloat(a.X,b.X)&&SameFloat(a.Y,b.Y)&&SameFloat(a.Z,b.Z));}
int main(){
 CEnvironmentSpellEffectInterface instance(0);owner=&instance;
 assert(instance.RadialBlurParams.Length==0&&instance.RadialBlurParams.Fade==0&&instance.RadialBlurPositionParams.AreaTrackPos==0&&instance.RadialBlurPositionParams.BlurTrackPos==0);
 const float fractions[]={-2,0,0.25f,0.5f,1,3};
 for(int n=0;n<24;++n){
 expectedFraction=fractions[n%6];calls=0;
 CBlurColour threshold={float(n+1),float(n+2),float(n+3),float(n+4)};
 CBlurColour intensity={float(n+5),float(n+6),float(n+7),float(n+8)};
 expectedParams.ThresholdColour=threshold;expectedParams.IntensityColour=intensity;
 expectedParams.Length=float(n+9);expectedParams.Fade=float(n+10);expectedParams.Range=float(n+11);expectedParams.Twist=float(n+12);
 CBlurVector area={float(n+13),float(n+14),float(n+15)},blur={float(n+16),float(n+17),float(n+18)};
 expectedPosition.AreaCenter=area;expectedPosition.BlurCenter=blur;expectedPosition.AreaTrackPos=n%3;expectedPosition.BlurTrackPos=(n+1)%3;
 CIDrawEnvironmentRadialBlurParameters result=instance.GetRenderFrameInterpolatedRadialBlurParameters(expectedFraction);
 assert(calls==2);CheckColour(result.ThresholdColour,threshold);CheckColour(result.IntensityColour,intensity);CheckVector(result.AreaCenter,area);CheckVector(result.BlurCenter,blur);
 assert(result.Length==expectedParams.Length&&result.Fade==expectedParams.Fade&&result.Range==expectedParams.Range&&result.Twist==expectedParams.Twist&&result.AreaTrackPos==expectedPosition.AreaTrackPos&&result.BlurTrackPos==expectedPosition.BlurTrackPos);
 }
 puts("008857e0_TEST PASS");return 0;}