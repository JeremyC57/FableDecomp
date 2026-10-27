// behaviour test for 0x0044C54D CCombatAbilityFlourishCounterAttackDef::CCombatAbilityFlourishCounterAttackDef() (model: the base receives the type constant)
#include <cstdio>
static int g_type = -1;
struct Base { Base(int t) { g_type = t; } };
struct Derived : Base { Derived() : Base(6) { } };
int main()
{
    Derived d; (void)d;
    int ok = g_type == 6;
    if (!ok) { std::printf("0044c54d_TEST FAIL\n"); return 1; }
    std::printf("0044c54d_TEST PASS\n");
    return 0;
}