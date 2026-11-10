// CGameScriptInterface::SetEnvironmentThemeWeightExternals(const CCharString& theme_name, float weight) const -- look the
//   environment theme up by definition name and, when it exists, set its weight on channel 0 at priority 1 of the world's environment
// Leaf: ?SetEnvironmentThemeWeightExternals@CGameScriptInterface@@UBEXABVCCharString@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00890090, 49 bytes, vtable slot 664, `ret 8`.
// Callees: CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CEnvironment::SetThemeWeight 0x006B3600 (Ego_r.pdb / FableWin.pdb ?SetThemeWeight@CEnvironment@@QAEXJJJM@Z).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CENVIRONMENT_SETTHEMEWEIGHT
#include <string.h>
#include "fable_string.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEnvironment.h"
#include "engine/CWorld.h"

void CGameScriptInterface::SetEnvironmentThemeWeightExternals(const CCharString& theme_name, float weight) const
{
    long theme = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(theme_name);
    if (theme > 0)
        World->PEnvironment_ptr->SetThemeWeight(0, 1, theme, weight);
}
