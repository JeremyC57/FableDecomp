#ifndef FABLE_CENVIRONMENTTHEMERESOURCEPALLETTE_H
#define FABLE_CENVIRONMENTTHEMERESOURCEPALLETTE_H
#include "engine/CBlendedEnvironmentTheme.h"
class CGameDefinitionManager;
namespace NEnvironmentInternal {
class CEnvironmentThemeSetDay {
    CArray<CBlendedEnvironmentTheme> Themes;
public:
    void Build(const CGameDefinitionManager&,const CEnvironmentDef&,const CEnvironmentLookupTexture&,const CEnvironmentThemeDaySetDef&);
    void GetBlendedTheme(CBlendedEnvironmentTheme&,float) const;
};
class CEnvironmentThemeResourcePallette {
    CArray<CEnvironmentThemeSetDay> DaySets;
public:
    void Build(const CGameDefinitionManager&,const CEnvironmentDef&,const CEnvironmentLookupTexture&);
    long GetCount() const;
    void GetBlendedTheme(CBlendedEnvironmentTheme& result,long index,float time) const {
        DaySets[index].GetBlendedTheme(result,time);
    }
};
}
#endif
