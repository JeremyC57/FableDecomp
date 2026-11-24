// Leaf: ?SetAllowScreenFadingIfAlreadyFaded@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

void CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded(bool value)
{
    CGameScriptInterface* self = this;
    // Retail stores the incoming byte verbatim; assigning through bool would
    // make VC7.1 normalize it to 0/1 and change the instruction sequence.
    *(char*)&self->AllowScreenFadingIfAlreadyFaded = value;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded -> CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
