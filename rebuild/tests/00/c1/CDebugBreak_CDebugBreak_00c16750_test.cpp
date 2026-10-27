// behaviour test for 00c16750 (standalone copy of the model)
#include <cstdio>
#include <cstring>

static int g_breaks = 0;
static void DebugBreakStub() { ++g_breaks; }
struct CDebugBreak { CDebugBreak() { DebugBreakStub(); } };

int main()
{
    CDebugBreak(); CDebugBreak b; (void)b;
    if (!(g_breaks == 2)) { std::printf("00c16750_TEST FAIL\n"); return 1; }
    std::printf("00c16750_TEST PASS\n");
    return 0;
}