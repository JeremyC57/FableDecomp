#include <stdio.h>
#include <string.h>
#include <new>
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

class CEnvironment{};
int main(){
 typedef CEnvironmentSpellEffectInterface Subject;
 if(sizeof(Subject)!=260||sizeof(Subject::CRadialBlurParams)!=48||sizeof(Subject::CRadialBlurPositionParams)!=32||sizeof(Subject::CScreenFilterParams)!=32)return 1;
 const float values[8]={-4.0f,-1.0f,0.0f,0.5f,1.0f,2.0f,4.0f,16.0f};
 const float vectors[4]={-4.0f,0.0f,0.5f,8.0f};
 const float filter[8]={0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f};
 CEnvironment owner;unsigned int cases=0;
 for(unsigned int pattern=0;pattern<4096;++pattern)for(unsigned int present=0;present<2;++present){
  union Storage{double align;unsigned char bytes[292];}storage;
  memset(storage.bytes,0xa5,sizeof(storage.bytes));
  for(unsigned int k=0;k<65;++k){float value=values[(k+pattern)%8];memcpy(storage.bytes+16+4*k,&value,4);}
  unsigned int bits=pattern;for(unsigned int k=0;k<6;++k){float v=vectors[bits&3];bits>>=2;memcpy(storage.bytes+16+52+4*k,&v,4);}
  unsigned char expected[260];memcpy(expected,storage.bytes+16,260);
  CEnvironment*environment=present?&owner:0;memcpy(expected,&environment,4);
  memset(expected+4,0,48);memset(expected+76,0,8);memset(expected+84,0,16);
  memcpy(expected+100,filter,32);memcpy(expected+132,expected+4,48);memcpy(expected+180,expected+52,32);memcpy(expected+228,filter,32);
  Subject*subject=new(storage.bytes+16)Subject(environment);
  if(memcmp(subject,expected,260))return 2;
  for(unsigned int k=0;k<16;++k)if(storage.bytes[k]!=0xa5||storage.bytes[276+k]!=0xa5)return 3;
  if(subject->Environment!=environment||subject->ScreenFilterParams.Saturation!=1.0f||subject->PrevFrameScreenFilterParams.Saturation!=1.0f)return 4;
  subject->~CEnvironmentSpellEffectInterface();++cases;
 }
 if(cases!=8192)return 5;puts("008852e0_TEST PASS");return 0;
}