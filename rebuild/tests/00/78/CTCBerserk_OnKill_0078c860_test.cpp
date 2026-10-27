// behaviour test for 0x0078C860: the component's method is called once
#include <cstdio>
static int g_calls = 0;
struct TC { void M() { ++g_calls; } };
struct Owner { TC* tc; void F() { TC* t = tc; t->M(); } };
int main()
{
    TC t; Owner o = { &t }; o.F();
    if (g_calls != 1) { std::printf("0078c860_TEST FAIL\n"); return 1; }
    std::printf("0078c860_TEST PASS\n");
    return 0;
}