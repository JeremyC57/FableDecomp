// CGameScriptInterface::TransitionToThemeExternals(const CCharString&, float) const -- look the theme up by name and, if it exists, blend the external environment channel (0) to it at script priority
// Leaf: ?TransitionToThemeExternals@CGameScriptInterface@@UBEXABVCCharString@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088FFB0, `ret 8`; vtable slot 660 of CGameScriptInterface (engine/CGameScriptInterface.h). The
//   sibling TransitionToThemeAllInternals 0x0088FF30 has the same body with a 1..4 channel loop.
// Callees: CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410,
//   CEnvironment::TransitionToTheme(long channel, long priority, long theme, float time) 0x006B3800 (Ego_r.pdb / FableWin.pdb
//   ?TransitionToTheme@CEnvironment@@QAEXJJJM@Z) on World (+0x04) -> PEnvironment (+0x18).
// NF122 (2026-11-20, q6229): ported from the free __fastcall stand-in (local Inner / Thing / extern "C" GetTheme models) onto the
//   shared headers; the old row bound 0x006B3800 as ?method@Inner@@QAEXHHHH@Z, 0x0044C6B0 as _GetTheme@4 and 0x009AD410 as
//   ?count@Thing@@QAEHXZ.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TRANSITIONTOTHEME
#include "fable_string.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CGameDefinitionManager.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::TransitionToThemeExternals(const CCharString& themeName, float blendTime) const
{
    const long globalIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(themeName);
    if (0 < globalIndex)
        World->GetEnvironment().TransitionToTheme(0, 1, globalIndex, blendTime);
}
