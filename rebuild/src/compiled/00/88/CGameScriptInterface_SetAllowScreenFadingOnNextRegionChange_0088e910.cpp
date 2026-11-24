// Leaf: ?SetAllowScreenFadingOnNextRegionChange@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public:
    unsigned char _pad[0xdb];
    unsigned char allowScreenFadingOnNextRegionChange; // +0xdb
};
void CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange(bool val)
{
    CGameScriptInterface* self = this;
    self->World->allowScreenFadingOnNextRegionChange = (unsigned char)val;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange -> CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
