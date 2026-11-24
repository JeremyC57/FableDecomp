// Leaf: ?CancelRadialBlurFade@CGameScriptInterface@@UBEXXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTarget;
extern void __fastcall Callee(CTarget* self);

struct CInner {
    char pad[0x1964];
    CTarget* fld;
};
class CWorld { public:
    char pad[0x18];
    CInner* inner;
};

void CGameScriptInterface::CancelRadialBlurFade() const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    Callee(self->World->inner->fld);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_CancelRadialBlurFade -> CGameScriptInterface::CancelRadialBlurFade, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
