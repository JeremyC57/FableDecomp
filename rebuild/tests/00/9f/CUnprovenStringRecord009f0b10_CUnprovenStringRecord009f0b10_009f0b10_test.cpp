// behaviour test for 0x009F0B10 CUnprovenStringRecord_009f0b10::CUnprovenStringRecord_009f0b10() (model: both members default-constructed in order)
#include <cstdio>
static int g_order = 0, g_first = 0, g_second = 0;
struct S1 { S1() { g_first = ++g_order; } };
struct S2 { S2() { g_second = ++g_order; } };
struct R { S1 a; S2 b; };
int main()
{
    R r; (void)r;
    int ok = g_first == 1 && g_second == 2;
    if (!ok) { std::printf("009f0b10_TEST FAIL\n"); return 1; }
    std::printf("009f0b10_TEST PASS\n");
    return 0;
}