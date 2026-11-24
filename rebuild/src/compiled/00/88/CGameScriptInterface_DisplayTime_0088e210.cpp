// Leaf: ?DisplayTime@CGameScriptInterface@@UBEX_N@Z
// CGameScriptInterface::DisplayTime @ 0x0088e210
// __fastcall, void return, 2 params (this + one stack arg). Body is empty: ret 0x4.
// Modeled as __fastcall: this in ecx, an unused edx slot, then the single stack arg.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::DisplayTime(bool display) const
{
    (void)display;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_DisplayTime -> CGameScriptInterface::DisplayTime, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
