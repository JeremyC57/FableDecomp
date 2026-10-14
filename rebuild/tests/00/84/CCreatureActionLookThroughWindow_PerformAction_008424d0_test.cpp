#include <cstdio>
static long g_mode = 0; static bool g_ex = true;
struct CPlayer { void AddMode(long m, bool e) { g_mode = m; g_ex = e; } };
static CPlayer g_player;
struct Base { virtual void PerformAction() {} CPlayer* GetPlayer() { return &g_player; } };
struct Act : Base { void PerformAction() { GetPlayer()->AddMode(16, false); } };
int main()
{
    Act a; Base* b = &a; b->PerformAction();
    if (g_mode != 16 || g_ex) { std::printf("008424d0_TEST FAIL\n"); return 1; }
    std::printf("008424d0_TEST PASS\n");
    return 0;
}