// Leaf: ?SetToDisplayTutorialsDuringCutscenes@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct GS2 { char pad[0xcc]; char fcc; };
class CUserProfileManager : public GS2 { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
void CGameScriptInterface::SetToDisplayTutorialsDuringCutscenes(bool arg) const
{ CUserProfileManager::Get().fcc = arg; }
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetToDisplayTutorialsDuringCutscenes -> CGameScriptInterface::SetToDisplayTutorialsDuringCutscenes, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
