#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: void SetEnabled(bool b); };

void __fastcall CGameScriptInterface_MiniMapSetAsEnabled(CGameScriptInterface* self, void* edx, bool enabled)
{
    (void)edx;
    self->World->SetEnabled(enabled);
}