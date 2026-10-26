#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner;
extern void __fastcall Inner_Do(Inner* self, int e, int a, int b, int c);
class CWorld { public: char pad[0x18]; Inner* PEnvironment; };

void __fastcall CGameScriptInterface_ResetToDefaultThemeExternals(CGameScriptInterface* self, int edx, int duration)
{
    Inner* obj = self->World->PEnvironment;
    Inner_Do(obj, edx, 0, 1, duration);
}