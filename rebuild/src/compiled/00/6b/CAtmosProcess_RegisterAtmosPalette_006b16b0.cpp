// 006b16b0 CAtmosProcess::RegisterAtmosPalette() -- register every sound theme and its atmosphere channels.
// Leaf: ?RegisterAtmosPalette@CAtmosProcess@@QAEXXZ
// Retail: 0x006B16B0,581 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
#define FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CSOUNDTHEMEDEF_PASS22
#define FABLE_RETAIL_STLP_MULTIMAP_PASS22_ATMOS_ITERATION
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSoundThemeDef.h"
#include "engine/CAtmosProcess.h"
void CAtmosProcess::RegisterAtmosPalette()
{
    if (PSoundManager) {
        PSoundManager->DeleteAllAtmoses();
        for (long i = 1; i < CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME"); ++i) {
            CDefPointer<const CSoundThemeDef> theme;
            CGameDefinitionManager::Get().GetDef("SOUND_THEME", i, &theme);
            long flags = 0;
            if (theme->PlayQuietlyWhenInside)
                flags = 0x2000;
            else if (theme->PlayQuietlyWhenOutside)
                flags = 0x4000;
            if (theme->PlayOnlyWhenInside)
                flags |= 0x800;
            else if (theme->PlayOnlyWhenOutside)
                flags |= 0x1000;
            PSoundManager->NewAtmos(theme->GetInstantiationName().GetString(), flags);
            for (std::multimap<CCharString,long>::const_iterator group = theme->AtmosGroupMap.begin(); group != theme->AtmosGroupMap.end(); ++group) {
                if (theme->AtmosIndex)
                    PSoundManager->NewChannel(theme->GetInstantiationName().GetString(), group->first, theme->AtmosIndex, group->second, theme->Gain);
            }
            PSoundManager->AddAtmos2Map(theme->GetInstantiationName().GetString(), i, theme->Granularity, theme->Gain, theme->Range);
        }
    }
}