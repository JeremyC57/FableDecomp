#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public:
    void Save(int a);
};


void __fastcall CGameScriptInterface_AutoSave(CGameScriptInterface* self)
{
    self->World->Save(0);
}