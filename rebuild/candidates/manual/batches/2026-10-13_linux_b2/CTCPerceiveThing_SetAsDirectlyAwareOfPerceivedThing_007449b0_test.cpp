#include <cstdio>

static int g_on = 0; static void* g_ret = 0;
struct CThing;
CThing* __fastcall IntelligentPointer_GetPItem_00a01b50(void* p) { return (CThing*)g_ret; }
struct P { unsigned char pad[0xc]; unsigned char Perceived[8]; unsigned char p2[8]; long Frames; unsigned char p3[0x10]; bool Direct;
    void On() { ++g_on; }
    void Set() { if (IntelligentPointer_GetPItem_00a01b50(&Perceived[0])) { Direct = true; Frames = 0; On(); } } };
int main()
{
    P p; p.Direct = false; p.Frames = 9; g_ret = 0; p.Set(); if (g_on != 0 || p.Direct) { std::printf("007449b0_TEST FAIL\n"); return 1; }
    int x; g_ret = &x; p.Set(); if (g_on != 1 || !p.Direct || p.Frames != 0) { std::printf("007449b0_TEST FAIL\n"); return 1; }
    std::printf("007449b0_TEST PASS\n");
    return 0;
}
