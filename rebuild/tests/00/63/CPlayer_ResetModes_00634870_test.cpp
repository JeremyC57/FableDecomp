#include <cstdio>

static int g_mode = -1; static int g_flag = -1;
struct CPlayer { unsigned char pad[0x208]; bool Local;
    void AddMode(long m, bool f) { g_mode = m; g_flag = f; }
    void ResetModes() { if (Local) AddMode(0, true); } };
int main()
{
    CPlayer p; p.Local = false; p.ResetModes(); if (g_mode != -1) { std::printf("00634870_TEST FAIL\n"); return 1; }
    p.Local = true; p.ResetModes(); if (g_mode != 0 || g_flag != 1) { std::printf("00634870_TEST FAIL\n"); return 1; }
    std::printf("00634870_TEST PASS\n");
    return 0;
}