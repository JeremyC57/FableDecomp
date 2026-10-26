#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner {
    void Apply_006b38d0(int one, int n, int duration);
};
class CWorld { public: unsigned char pad[0x18]; Inner* PEnvironment; };

extern void* __stdcall Resolve(int a);
extern int __fastcall CountOf(void* self);

void __fastcall CGameScriptInterface_TransitionToTheme(CGameScriptInterface* self, void* edx, int name, int duration)
{
    void* r = Resolve(name);
    int n = CountOf(r);
    if (n > 0) {
        self->World->PEnvironment->Apply_006b38d0(1, n, duration);
    }
}