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
// CEngineVideoOptionsDef::Transfer(CPersistContext&) -- transfers video options in persistence order.
// Retail 00434120, 244 bytes. Donor _quarantine/CEngineVideoOptionsDef.h
// has a 0x40-byte base; native first member is +0x3c (all members -4).
// Honest retail view uses the donor names; no native virtual calls occur here.
#include <stddef.h>
class CPersistContext { public:
 template <class T> void Transfer(T& value); // 00431102
};
template <> void CPersistContext::Transfer<long>(long& value);
template <> void CPersistContext::Transfer<float>(float& value);
template <> void CPersistContext::Transfer<bool>(bool& value);
class CEngineVideoOptionsDef { public:
 unsigned char DefinitionBase[0x3c];
 int HiresTextureMemory;
 float LODErrorTolerance, CharacterLODErrorTolerance, DrawDistanceMultiplier;
 float DrawDistanceMinimum, DrawDistanceMaximum, RepeatedMeshDrawDistanceFactor;
 float MinimumZSpriteAsMeshDistance, MaximumZSpriteAsMeshDistance, ZSpriteDrawDistanceMultiplier;
 int ShadowBufferSize;
 float ShadowDistanceScale;
 bool Enable2DDisplacement, Enable3DDisplacement, EnableGlow, EnableRadialBlur;
 bool EnableWaterReflection, EnableWeatherEffects, EnableColourFilter, EnableRepeatedMeshes;
 float WeatherDensity;
 void Transfer(CPersistContext& c);
};
typedef char NativeVideoSize[(sizeof(CEngineVideoOptionsDef)==0x78)?1:-1];
typedef char NativeVideoOffset[(offsetof(CEngineVideoOptionsDef,HiresTextureMemory)==0x3c)?1:-1];
void CEngineVideoOptionsDef::Transfer(CPersistContext& c) {
 c.Transfer(*(long*)&(HiresTextureMemory));
 c.Transfer(LODErrorTolerance);
 c.Transfer(CharacterLODErrorTolerance);
 c.Transfer(DrawDistanceMultiplier);
 c.Transfer(DrawDistanceMinimum);
 c.Transfer(DrawDistanceMaximum);
 c.Transfer(RepeatedMeshDrawDistanceFactor);
 c.Transfer(MinimumZSpriteAsMeshDistance);
 c.Transfer(MaximumZSpriteAsMeshDistance);
 c.Transfer(ZSpriteDrawDistanceMultiplier);
 c.Transfer(*(long*)&(ShadowBufferSize));
 c.Transfer(ShadowDistanceScale);
 c.Transfer(Enable2DDisplacement);
 c.Transfer(Enable3DDisplacement);
 c.Transfer(EnableGlow);
 c.Transfer(EnableRadialBlur);
 c.Transfer(EnableWaterReflection);
 c.Transfer(EnableWeatherEffects);
 c.Transfer(EnableColourFilter);
 c.Transfer(WeatherDensity);
 c.Transfer(EnableRepeatedMeshes);
}



#include <stdio.h>
static void* expected[21]; static int types[21], n, bad, writing;
static void Check(void* p,int type) { if(n>=21 || expected[n]!=p || types[n]!=type) bad=1; ++n; }
template <> void CPersistContext::Transfer<long>(long& value_)
{
    int& v = *(int*)&value_; (void)&v;Check(&v,1); if(writing) v=100+n; else if(v!=100+n)bad=1;}
template <> void CPersistContext::Transfer<float>(float& value_)
{
    float& v = *(float*)&value_; (void)&v;Check(&v,2);if(writing) v=0.25f+n;else if(v!=0.25f+n)bad=1;}
template <> void CPersistContext::Transfer<bool>(bool& value_)
{
    bool& v = *(bool*)&value_; (void)&v;Check(&v,3);if(writing) v=(n%2)!=0;else if(v!=((n%2)!=0))bad=1;}
int main(){ CEngineVideoOptionsDef x; CPersistContext c;
 expected[0]=&x.HiresTextureMemory; types[0]=1;
 expected[1]=&x.LODErrorTolerance; types[1]=2;
 expected[2]=&x.CharacterLODErrorTolerance; types[2]=2;
 expected[3]=&x.DrawDistanceMultiplier; types[3]=2;
 expected[4]=&x.DrawDistanceMinimum; types[4]=2;
 expected[5]=&x.DrawDistanceMaximum; types[5]=2;
 expected[6]=&x.RepeatedMeshDrawDistanceFactor; types[6]=2;
 expected[7]=&x.MinimumZSpriteAsMeshDistance; types[7]=2;
 expected[8]=&x.MaximumZSpriteAsMeshDistance; types[8]=2;
 expected[9]=&x.ZSpriteDrawDistanceMultiplier; types[9]=2;
 expected[10]=&x.ShadowBufferSize; types[10]=1;
 expected[11]=&x.ShadowDistanceScale; types[11]=2;
 expected[12]=&x.Enable2DDisplacement; types[12]=3;
 expected[13]=&x.Enable3DDisplacement; types[13]=3;
 expected[14]=&x.EnableGlow; types[14]=3;
 expected[15]=&x.EnableRadialBlur; types[15]=3;
 expected[16]=&x.EnableWaterReflection; types[16]=3;
 expected[17]=&x.EnableWeatherEffects; types[17]=3;
 expected[18]=&x.EnableColourFilter; types[18]=3;
 expected[19]=&x.WeatherDensity; types[19]=2;
 expected[20]=&x.EnableRepeatedMeshes; types[20]=3;
 writing=1;x.Transfer(c);if(n!=21)bad=1;n=0;writing=0;x.Transfer(c);if(n!=21)bad=1; printf("00434120_TEST %s\n",bad?"FAIL":"PASS");return bad; }