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