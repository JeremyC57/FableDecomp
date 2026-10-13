#include <cstdio>

static int g_base = 0;
struct CClickable { unsigned char p0[0x30]; long Configured; unsigned char p1[0x15C - 0x34]; long Current;
    void InitialiseBase() { g_base = 1; }
    void Initialise() { InitialiseBase(); Current = Configured; } };
int main()
{
    CClickable c; c.Configured = 42; c.Current = 0; c.Initialise();
    if (g_base != 1 || c.Current != 42) { std::printf("00546690_TEST FAIL\n"); return 1; }
    std::printf("00546690_TEST PASS\n");
    return 0;
}
