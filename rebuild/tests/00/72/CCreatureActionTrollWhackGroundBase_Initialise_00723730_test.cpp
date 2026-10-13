#include <cstdio>

static int g_order = 0;
struct V { virtual void Prepare() { g_order = g_order * 10 + 1; } };
struct Action : V { void InitialiseBase() { g_order = g_order * 10 + 2; }
    void Initialise() { Prepare(); InitialiseBase(); } };
int main()
{
    Action a; a.Initialise();
    if (g_order != 12) { std::printf("00723730_TEST FAIL\n"); return 1; }
    std::printf("00723730_TEST PASS\n");
    return 0;
}