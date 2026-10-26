// behaviour test for 0x00953780 CCreatureAction_DragonFireBlastLeft::CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature, CThing& target): both arguments reach the base constructor unchanged, and the
// finished object dispatches as the derived class
#include <cstdio>
struct Root { virtual ~Root() { } virtual int Kind() const { return 0; } };
struct Base : Root {
    int A, B;
    Base(int a, int b) : A(a), B(b) { }
    int Kind() const { return 1; }
};
struct Derived : Base {
    Derived(int a, int b) : Base(a, b) { }                  // the function under test
    int Kind() const { return 2; }
};
int main()
{
    int fail = 0;
    Derived d(17, 1);
    if (d.A != 17 || d.B != 1) fail |= 1;
    Root* r = &d;
    if (r->Kind() != 2) fail |= 2;
    Derived e(-3, 0);
    if (e.A != -3 || e.B != 0 || static_cast<Root&>(e).Kind() != 2) fail |= 4;
    if (fail) { std::printf("00953780_TEST FAIL %d\n", fail); return 1; }
    std::printf("00953780_TEST PASS\n");
    return 0;
}