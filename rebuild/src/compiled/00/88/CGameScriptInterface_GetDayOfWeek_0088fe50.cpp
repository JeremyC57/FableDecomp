// Leaf: ?GetDayOfWeek@CGameScriptInterface@@UBEJXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner2 { char _p[0x14]; long f14; };
class CWorld { public: char _p[0x1c]; Inner2* PGameTimeManager; };

long CGameScriptInterface::GetDayOfWeek() const
{
    const CGameScriptInterface* self = this;
    CWorld* m = self->World;
    Inner2* x = m->PGameTimeManager;
    return x->f14 % 7;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_GetDayOfWeek -> CGameScriptInterface::GetDayOfWeek, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
