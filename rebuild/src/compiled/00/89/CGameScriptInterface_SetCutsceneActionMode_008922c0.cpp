// Leaf: ?SetCutsceneActionMode@CGameScriptInterface@@UBEX_NABVCCharString@@@Z
#define FABLE_RETAIL_NSCRIPT   // alias route: engine/CGameScriptInterface.h declares SetCutsceneActionMode under this class-model switch
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString;
struct Inner { void SetMode(bool, const CCharString&); };
extern Inner* g_cutscene;
void CGameScriptInterface::SetCutsceneActionMode(bool a, const CCharString& name) const {
    ((Inner*)((char*)g_cutscene + 0x18))->SetMode(a, name);
}