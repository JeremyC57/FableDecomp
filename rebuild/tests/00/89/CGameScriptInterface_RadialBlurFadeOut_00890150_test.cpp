#include "engine/CGameScriptInterface.h"
#include <cstdio>

static void* g_this = 0;
static int g_a = 0, g_b = 0;

struct Tgt { void method(int duration, int b); };
void Tgt::method(int duration, int b){ g_this = this; g_a = duration; g_b = b; }

class CWorld { public: char pad[0x18]; char* PEnvironment; };

void __fastcall CGameScriptInterface_RadialBlurFadeOut(CGameScriptInterface* self, int edx, int duration, int b)
{
    char* mid = self->World->PEnvironment;
    Tgt* t = *(Tgt**)(mid + 0x1964);
    t->method(duration, b);
}

int main(){
    char midblock[0x1968];
    Tgt tgtObj;
    *(void**)(midblock + 0x1964) = (void*)&tgtObj;
    CWorld b; b.PEnvironment = midblock;
    CGameScriptInterface self; self.World = &b;
    CGameScriptInterface_RadialBlurFadeOut(&self, 0, 111, 222);
    if (g_this == (void*)&tgtObj && g_a == 111 && g_b == 222) {
        std::printf("CGameScriptInterface_00890150_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}