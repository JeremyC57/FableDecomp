// Leaf: ?AutoSave@CGameScriptInterface@@UBEXXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public:
    void Save(int a);
};


void CGameScriptInterface::AutoSave() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    self->World->Save(0);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_AutoSave -> CGameScriptInterface::AutoSave, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
