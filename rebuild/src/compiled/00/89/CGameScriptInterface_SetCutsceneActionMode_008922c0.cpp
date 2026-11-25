// Leaf: ?SetCutsceneActionMode@CGameScriptInterface@@UBEX_NABVCCharString@@@Z
#define FABLE_RETAIL_NSCRIPT   // alias route: engine/CGameScriptInterface.h declares SetCutsceneActionMode under this class-model switch
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString;
struct Inner { void SetMode(bool, const CCharString&); };
extern Inner* g_cutscene;
// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004908DA ?SetCutsceneActionMode@CSubtitleRenderer@@QAEX_NABVCCharString@@@Z (was ?SetMode@Inner@@QAEX_NABVCCharString@@@Z)
class CSubtitleRenderer { public: void SetCutsceneActionMode(bool, CCharString const&); };
void CGameScriptInterface::SetCutsceneActionMode(bool a, const CCharString& name) const {
    reinterpret_cast<CSubtitleRenderer*>(((Inner*)((char*)g_cutscene + 0x18)))->SetCutsceneActionMode(a, name);
}