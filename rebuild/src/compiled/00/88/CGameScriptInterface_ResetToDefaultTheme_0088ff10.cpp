#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInner {
    void Method(int a, int b);
};
struct CWorld {
    char pad[0x18];
    CInner* PEnvironment;
};

void __fastcall CGameScriptInterface_ResetToDefaultTheme(CGameScriptInterface* self, int edx, int duration)
{
    self->World->PEnvironment->Method(1, duration);
}