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
#ifdef FABLE_RETAIL_CENVIRONMENTTHEMERESOURCEPALLETTE_GETCOUNT
    // Retail opt-in (chunk 1021, row 0x006B3E60): FableWin.pdb ?GetCount@CEnvironmentThemeResourcePallette@NEnvironmentInternal@@QBEJXZ
    // (no Ego_r.pdb copy, expanded in retail: 0x006B3F97..0x006B3FB3, (end - begin) / 12 through 0x2AAAAAAB).
    long GetCount() const { return DaySets.size(); }
#else
    long GetCount() const;
#endif
    void GetBlendedTheme(CBlendedEnvironmentTheme& result,long index,float time) const {
        DaySets[index].GetBlendedTheme(result,time);
    }
};
}
#endif
