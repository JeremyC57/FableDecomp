#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner { void Apply(int one, int count, int arg2); };
class CWorld { public: char pad[0x18]; Inner* PEnvironment; };

struct ThemeObj;
extern ThemeObj* __stdcall GetTheme(int channel);
extern int __fastcall ThemeCount(ThemeObj* self);

void __fastcall CGameScriptInterface_SetEnvironmentThemeWeightAllChannels(
    CGameScriptInterface* self, int /*edx*/, int channel, int arg2)
{
    ThemeObj* t = GetTheme(channel);
    int n = ThemeCount(t);
    if (n > 0) {
        self->World->PEnvironment->Apply(1, n, arg2);
    }
}