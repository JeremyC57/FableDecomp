#include "engine/CGameScriptInterface.h"
#include <cstdio>

struct CInner {
    int val;
    void Method(int a, int b) { val = a + b; }
};
class CWorld { public:
    char pad[0x18];
    CInner* PEnvironment;
};

void __fastcall CGameScriptInterface_ResetToDefaultTheme(CGameScriptInterface* self, int edx, int duration)
{
    self->World->PEnvironment->Method(1, duration);
}

int main() {
    CInner inner; inner.val = 0;
    CWorld theMid; theMid.PEnvironment = &inner;
    CGameScriptInterface obj; obj.World = &theMid;
    CGameScriptInterface_ResetToDefaultTheme(&obj, 0, 41);
    if (inner.val == 42) { std::printf("CGameScriptInterface_0088ff10_TEST PASS\n"); return 0; }
    std::printf("FAIL got %d\n", inner.val); return 1;
}