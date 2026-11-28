#include <assert.h>
#include <stdio.h>
struct FixtureCreature {
    int MeleeCalls;
    FixtureCreature(): MeleeCalls(0) {}
    void UnsheatheMeleeWeapon() { ++MeleeCalls; }
    void UnsheathePrimaryWeapon() { UnsheatheMeleeWeapon(); }
};
int main()
{
    FixtureCreature one, two;
    one.UnsheathePrimaryWeapon();
    assert(one.MeleeCalls==1 && two.MeleeCalls==0);
    puts("00666820_TEST PASS");
    return 0;
}
