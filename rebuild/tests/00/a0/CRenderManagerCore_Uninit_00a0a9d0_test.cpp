#include <cstdio>

static int g_un = 0;
struct Font { bool init; bool IsInitialised() { return init; } void Uninitialise() { ++g_un; } };
struct RM { unsigned char pad[0x3a4c]; Font SystemFont;
    void Uninit() { Font* f = &SystemFont; if (f->IsInitialised()) f->Uninitialise(); } };
int main()
{
    RM* rm = new RM; rm->SystemFont.init = false; rm->Uninit(); if (g_un != 0) { std::printf("00a0a9d0_TEST FAIL\n"); return 1; }
    rm->SystemFont.init = true; rm->Uninit(); if (g_un != 1) { std::printf("00a0a9d0_TEST FAIL\n"); return 1; }
    delete rm;
    std::printf("00a0a9d0_TEST PASS\n");
    return 0;
}