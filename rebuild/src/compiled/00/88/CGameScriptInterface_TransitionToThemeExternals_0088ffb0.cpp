#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner {
    void method(int a, int b, int c, int d);
};

struct Thing {
    int count();
};

struct CWorld {
    unsigned char pad[0x18];
    Inner* PEnvironment; // +0x18
};


extern "C" Thing* __stdcall GetTheme(int name);

void __fastcall CGameScriptInterface_TransitionToThemeExternals(CGameScriptInterface* self, int edx_unused, int name, int duration)
{
    Thing* t = GetTheme(name);
    int n = t->count();
    if (n > 0) {
        self->World->PEnvironment->method(0, 1, n, duration);
    }
}