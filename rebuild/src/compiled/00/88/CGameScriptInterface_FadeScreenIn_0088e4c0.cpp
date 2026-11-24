// Leaf: ?FadeScreenIn@CGameScriptInterface@@UAEXXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CDisplayEngine { public: char pad[0xd8]; unsigned char flag; };
extern void __fastcall CGameScriptInterface_DoFade(CGameScriptInterface* self);
void CGameScriptInterface::FadeScreenIn()
{
    CGameScriptInterface* self = this;
    self->DisplayEngine->flag = 0;
    CGameScriptInterface_DoFade(self);
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_FadeScreenIn -> CGameScriptInterface::FadeScreenIn, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
