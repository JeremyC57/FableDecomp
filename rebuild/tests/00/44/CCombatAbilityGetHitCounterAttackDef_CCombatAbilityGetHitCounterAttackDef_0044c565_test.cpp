// behaviour test for 0x0044C565 CCombatAbilityGetHitCounterAttackDef::CCombatAbilityGetHitCounterAttackDef() (model: the base receives the type constant)
#include <cstdio>
static int g_type = -1;
struct Base { Base(int t) { g_type = t; } };
struct Derived : Base { Derived() : Base(9) { } };
int main()
{
    Derived d; (void)d;
    int ok = g_type == 9;
    if (!ok) { std::printf("0044c565_TEST FAIL\n"); return 1; }
    std::printf("0044c565_TEST PASS\n");
    return 0;
}