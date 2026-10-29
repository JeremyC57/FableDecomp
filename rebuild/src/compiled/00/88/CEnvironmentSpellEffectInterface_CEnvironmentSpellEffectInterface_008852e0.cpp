// 008852e0 CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface
// Flags:
// Leaf: ??0CEnvironmentSpellEffectInterface@@QAE@PAVCEnvironment@@@Z
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CEnvironment;
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;C3DVector(){}};
#pragma pack(pop)
class CRGBFloatColour{public:float R,G,B,A;CRGBFloatColour(){}
 __forceinline void Initialise(float r,float g,float b,float a){R=r;G=g;B=b;A=a;}
};
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION{};
// Layout: no engine header for this class. Donor offsets8952..8980 and native
// writes agree260bytes. Displacement blocks have16opaque bytes because their
// original nested type's class key is not proved; do not invent a declaration.
class CEnvironmentSpellEffectInterface{public:
 struct CRadialBlurParams{CRGBFloatColour ThresholdColour,IntensityColour;float Length,Fade,Range,Twist;CRadialBlurParams(){}};
 struct CRadialBlurPositionParams{C3DVector AreaCenter,BlurCenter;ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos,BlurTrackPos;CRadialBlurPositionParams(){}};
 struct CScreenFilterParams{float Alpha,Saturation,Contrast,Brightness;CRGBFloatColour Tint;
  __forceinline CScreenFilterParams(){SetToDefault();}
  __forceinline void SetToDefault(){Alpha=Contrast=Brightness=0.0f;Saturation=1.0f;Tint.Initialise(1.0f,1.0f,1.0f,1.0f);}
 };
 CEnvironment*Environment;CRadialBlurParams RadialBlurParams;CRadialBlurPositionParams RadialBlurPositionParams;
 unsigned char DisplacementParams[16];CScreenFilterParams ScreenFilterParams;
 CRadialBlurParams PrevFrameRadialBlurParams;CRadialBlurPositionParams PrevFrameRadialBlurPositionParams;
 unsigned char PrevFrameDisplacementParams[16];CScreenFilterParams PrevFrameScreenFilterParams;
 CEnvironmentSpellEffectInterface(CEnvironment*);
};
CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface(CEnvironment*environment){
 Environment=environment;
 RadialBlurParams.Length=RadialBlurParams.Fade=0.0f;RadialBlurParams.Range=RadialBlurParams.Twist=0.0f;
 RadialBlurParams.ThresholdColour.Initialise(0.0f,0.0f,0.0f,0.0f);
 RadialBlurParams.IntensityColour.Initialise(0.0f,0.0f,0.0f,0.0f);
 RadialBlurPositionParams.AreaTrackPos=static_cast<ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION>(0);
 RadialBlurPositionParams.BlurTrackPos=static_cast<ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION>(0);
 PrevFrameRadialBlurParams=RadialBlurParams;PrevFrameRadialBlurPositionParams=RadialBlurPositionParams;
 CRGBFloatColour&colour=*reinterpret_cast<CRGBFloatColour*>(DisplacementParams);
 colour.R=0.0f;colour.G=0.0f;colour.B=0.0f;colour.A=0.0f;
}