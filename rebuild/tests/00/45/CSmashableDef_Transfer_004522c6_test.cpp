#include <cstdio>
#include "engine/CSmashableDef.h"

static unsigned char* g_a; static unsigned char* g_b; static int* g_c;

class CPersistContext { public:
    int calls;
    void PersistA(unsigned char* p){ calls++; g_a=p; }
    void PersistB(unsigned char* p){ calls++; g_b=p; }
    void PersistC(int* p){ calls++; g_c=p; }
};

void __fastcall CSmashableDef_Transfer(CSmashableDef* def, int edx, CPersistContext* ctx)
{
    ctx->PersistA(reinterpret_cast<unsigned char*>(&def->Smashable));
    ctx->PersistB(reinterpret_cast<unsigned char*>(&def->ReplacementObject_Val));
    ctx->PersistC(reinterpret_cast<int*>(&def->SmashParticleEmitter));
}

int main(){
    CSmashableDef d; CPersistContext c; c.calls=0;
    CSmashableDef_Transfer(&d, 0, &c);
    if (c.calls==3 && g_a==reinterpret_cast<unsigned char*>(&d.Smashable)
        && g_b==reinterpret_cast<unsigned char*>(&d.ReplacementObject_Val)
        && g_c==reinterpret_cast<int*>(&d.SmashParticleEmitter)){
        std::printf("CSmashableDef_004522c6_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
