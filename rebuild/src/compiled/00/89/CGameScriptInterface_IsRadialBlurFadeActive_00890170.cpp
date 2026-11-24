// Leaf: ?IsRadialBlurFadeActive@CGameScriptInterface@@UBE_NXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner { char pad[0x78]; bool flag; };
struct Mid { char pad[0x1964]; Inner* inner; };
class CWorld { public: char pad[0x18]; Mid* mid; };

bool CGameScriptInterface::IsRadialBlurFadeActive() const
{
    const CGameScriptInterface* self = this;
    return self->World->mid->inner->flag;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_IsRadialBlurFadeActive -> CGameScriptInterface::IsRadialBlurFadeActive, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
