#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Tgt { void method(int duration, int b); };
struct CWorld { char pad[0x18]; char* PEnvironment; };

void __fastcall CGameScriptInterface_RadialBlurFadeOut(CGameScriptInterface* self, int edx, int duration, int b)
{
    char* mid = self->World->PEnvironment;
    Tgt* t = *(Tgt**)(mid + 0x1964);
    t->method(duration, b);
}