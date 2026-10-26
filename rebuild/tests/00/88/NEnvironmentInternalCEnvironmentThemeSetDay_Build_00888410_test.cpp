// NEnvironmentInternal::CEnvironmentThemeSetDay::Build(...) -- resize and build each daily blended theme.
// Retail00888410 180 bytes; default theme ctor008864a0, dtor006b5d80,
// CArray::Resize0088a520, blended theme Build008865c0.
// Donor day/theme definitions have a four-byte larger base. Native TimeTheme+3c,
// native ColourLookupColumn+60 (12-byte CArray vs donor16). Blended theme500 vs donor524.
#ifdef _MSC_VER
#define CEnvironmentThemeDaySetDef CEnvironmentThemeDaySetDefDonor
#include "engine/CEnvironmentThemeDaySetDef.h"
#undef CEnvironmentThemeDaySetDef
#define CEnvironmentThemeDef CEnvironmentThemeDefDonor
#include "engine/_quarantine/CEnvironmentThemeDef.h"
#undef CEnvironmentThemeDef
#endif
#include <stddef.h>
class CGameDefinitionManager; class CEnvironmentDef; struct CEnvironmentLookupTexture;
struct CEnvironmentThemeDefRetail { unsigned char DefinitionBase[60];float TimeOfDay;unsigned char Parameters[132]; };
template<class T>struct CArrayRetail {
 T* First;T* Last;T* Capacity;
 long Size()const{return Last-First;} T& operator[](long i){return First[i];}
 void Resize(long,const T&);
};
struct CEnvironmentThemeDaySetDefRetail {
 unsigned char DefinitionBase[60]; CArrayRetail<CEnvironmentThemeDefRetail> TimeTheme;
 float SunTilt,SunRotate,SunHeight,MoonTilt,MoonRotate,MoonHeight;
 long ColourLookupColumn;
};
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme { public:
 float TimeOfDay;float Colours[19][4];float Scalers[30];
 struct CIDrawEnvironmentTextureSet {CArrayRetail<long> Textures;CArrayRetail<float> Blends;} TextureSets[3];
 CBlendedEnvironmentTheme();~CBlendedEnvironmentTheme();
 void Build(CEnvironmentDef*,CEnvironmentLookupTexture*,CEnvironmentThemeDaySetDefRetail*,CEnvironmentThemeDefRetail*,long);
};
class CEnvironmentThemeSetDay { public:
 CArrayRetail<CBlendedEnvironmentTheme> Themes;
 void Build(CGameDefinitionManager*,CEnvironmentDef*,CEnvironmentLookupTexture*,CEnvironmentThemeDaySetDefRetail*);
};
void CEnvironmentThemeSetDay::Build(CGameDefinitionManager* manager,CEnvironmentDef* env,CEnvironmentLookupTexture* lookup,CEnvironmentThemeDaySetDefRetail* day) {
 long count=day->TimeTheme.Size();Themes.Resize(count,CBlendedEnvironmentTheme());
 for(long i=0;i<count;++i)Themes[i].Build(env,lookup,day,&day->TimeTheme[i],day->ColourLookupColumn+i);
}
}

#include <stdio.h>
using namespace NEnvironmentInternal;
class CEnvironmentDef { public:int marker;};struct CEnvironmentLookupTexture {int marker;};
static int ctorCount,dtorCount,buildCount,resizeCount,expectedCount;static bool ok=true;
static CEnvironmentDef* expectedEnv;static CEnvironmentLookupTexture* expectedLookup;
static CEnvironmentThemeDaySetDefRetail* expectedDay;
static CBlendedEnvironmentTheme* destination;static CEnvironmentThemeSetDay* owner;
static CEnvironmentThemeDefRetail* firstThemes;static CEnvironmentThemeDefRetail* otherThemes;
static CBlendedEnvironmentTheme* otherDestination;
CBlendedEnvironmentTheme::CBlendedEnvironmentTheme(){++ctorCount;TimeOfDay=-17;}
CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme(){++dtorCount;}
template<class T> void CArrayRetail<T>::Resize(long n,const T& value){
 ++resizeCount;ok=ok&&n==expectedCount&&value.TimeOfDay==-17&&ctorCount==1&&dtorCount==0;
 First=destination;Last=destination+n;Capacity=Last;
}
void CBlendedEnvironmentTheme::Build(CEnvironmentDef* e,CEnvironmentLookupTexture* l,CEnvironmentThemeDaySetDefRetail* d,CEnvironmentThemeDefRetail* t,long col){
 int i=buildCount++;ok=ok&&e==expectedEnv&&l==expectedLookup&&d==expectedDay&&dtorCount==1;
 if(i==0){ok=ok&&this==destination&&t==firstThemes&&col==20;d->TimeTheme.First=otherThemes;d->ColourLookupColumn=40;owner->Themes.First=otherDestination;}
 else ok=ok&&this==otherDestination+i&&t==otherThemes+i&&col==40+i;
 TimeOfDay=t->TimeOfDay;
}
static void run(int count,CBlendedEnvironmentTheme* out,CBlendedEnvironmentTheme* alt,CEnvironmentThemeDefRetail* in,CEnvironmentThemeDefRetail* next){
 CEnvironmentDef env;CEnvironmentLookupTexture lookup;CEnvironmentThemeDaySetDefRetail day;CEnvironmentThemeSetDay set;
 day.TimeTheme.First=in;day.TimeTheme.Last=in+count;day.TimeTheme.Capacity=in+count;day.ColourLookupColumn=20;
 expectedEnv=&env;expectedLookup=&lookup;expectedDay=&day;owner=&set;destination=out;otherDestination=alt;firstThemes=in;otherThemes=next;expectedCount=count;
 ctorCount=dtorCount=buildCount=resizeCount=0;set.Build(0,&env,&lookup,&day);
 ok=ok&&ctorCount==1&&dtorCount==1&&resizeCount==1&&buildCount==count;
}
int main(){
 CBlendedEnvironmentTheme out[3],alt[3];CEnvironmentThemeDefRetail in[3],next[3];for(int i=0;i<3;++i){in[i].TimeOfDay=(float)i;next[i].TimeOfDay=(float)(i+10);}
 if(sizeof(void*)==4)ok=sizeof(CBlendedEnvironmentTheme)==500&&sizeof(CEnvironmentThemeDefRetail)==196&&offsetof(CEnvironmentThemeDaySetDefRetail,TimeTheme)==60&&offsetof(CEnvironmentThemeDaySetDefRetail,ColourLookupColumn)==96;
 run(0,out,alt,in,next);run(1,out,alt,in,next);run(3,out,alt,in,next);
 ok=ok&&out[0].TimeOfDay==0&&alt[1].TimeOfDay==11&&alt[2].TimeOfDay==12;
 printf(ok?"00888410_TEST PASS\n":"00888410_TEST FAIL\n");return ok?0:1;
}