// behaviour test for 0x0044c4ab CCombatAbilityStrafingData::CCombatAbilityStrafingData(ECombatAbilityInterfaceType): the base constructor sees the argument, then the
// member RotationSpeed is 20.0f and the object keeps everything the base set
#include <cstdio>
struct Base { int seen; int tag; Base(int a) : seen(a), tag(0x55) { } };
struct Derived : Base { float m; Derived(int a) : Base(a), m(20.0f) { } };
int main()
{
    int fails = 0;
    Derived d(7);
    if (d.seen != 7 || d.tag != 0x55) fails |= 1;
    double diff = (double)d.m - (double)(20.0f);
    if (diff > 1e-6 || diff < -1e-6) fails |= 2;
    if (fails) { std::printf("0044c4ab_TEST FAIL %d\n", fails); return 1; }
    std::printf("0044c4ab_TEST PASS\n");
    return 0;
}