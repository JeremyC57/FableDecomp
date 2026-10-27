// behaviour test for 0x008CF8F0 CCreatureAction_TentacleStrikeGetHit::CCreatureAction_TentacleStrikeGetHit() (model: the base receives the type constant)
#include <cstdio>
static int g_type = -1;
struct Base { Base(int t) { g_type = t; } };
struct Derived : Base { Derived() : Base(150) { } };
int main()
{
    Derived d; (void)d;
    int ok = g_type == 150;
    if (!ok) { std::printf("008cf8f0_TEST FAIL\n"); return 1; }
    std::printf("008cf8f0_TEST PASS\n");
    return 0;
}