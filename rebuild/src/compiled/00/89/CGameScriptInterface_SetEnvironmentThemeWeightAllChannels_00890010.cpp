// CGameScriptInterface::SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const -- look the theme up by name and, if it exists, set its weight on every environment channel at script priority (1)
// Leaf: ?SetEnvironmentThemeWeightAllChannels@CGameScriptInterface@@UBEXABVCCharString@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890010, `ret 8`; vtable slot 662 of CGameScriptInterface (engine/CGameScriptInterface.h); Ego_r.pdb
//   ?SetEnvironmentThemeWeightAllChannels@CGameScriptInterface@@UBEXABVCCharString@@M@Z. The siblings TransitionToThemeExternals 0x0088FFB0 (landed, NF122)
//   and TransitionToTheme 0x0088FEE0 have the same shape.
// Callees: CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410,
//   CEnvironment::SetThemeWeightAllChannels(long priority, long theme, float weight) 0x006B3680 (Ego_r.pdb / FableWin.pdb
//   ?SetThemeWeightAllChannels@CEnvironment@@QAEXJJM@Z) on World (+0x04) -> PEnvironment (+0x18).
// NF132 (q6233): ported from the free __fastcall stand-in (local Inner / CWorld models, GetTheme / ThemeCount externs) onto the shared
//   headers, the 0x0088FFB0 recipe; the old row bound 0x0044C6B0 as ?GetTheme@@YGPAUThemeObj@@H@Z, 0x009AD410 as ?ThemeCount@@YIHPAUThemeObj@@@Z and
//   0x006B3680 as ?Apply@Inner@@QAEXHHH@Z.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SETTHEMEWEIGHTALLCHANNELS
#include "fable_string.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CGameDefinitionManager.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetEnvironmentThemeWeightAllChannels(const CCharString& themeName, float weight) const
{
    const long globalIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(themeName);
    if (0 < globalIndex)
        World->GetEnvironment().SetThemeWeightAllChannels(1, globalIndex, weight);
}
