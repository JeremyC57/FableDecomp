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