// Leaf: ?SetHUDEnabled@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CHud;
extern CHud* g_hud;
void __fastcall Hud_Enable(CHud* self);
void __fastcall Hud_Disable(CHud* self);

void CGameScriptInterface::SetHUDEnabled(bool enabled) const
{
    if (enabled)
        Hud_Enable(g_hud);
    else
        Hud_Disable(g_hud);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetHUDEnabled -> CGameScriptInterface::SetHUDEnabled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
