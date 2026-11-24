// Leaf: ?GetDayCount@CGameScriptInterface@@UBEJXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner2 { unsigned char _p[0x14]; long day; };
class CWorld { public: unsigned char _p[0x1c]; Inner2* PGameTimeManager; };

long CGameScriptInterface::GetDayCount() const
{
    const CGameScriptInterface* self = this;
    return self->World->PGameTimeManager->day;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetDayCount -> CGameScriptInterface::GetDayCount, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
