// behaviour test for 0x004dca62 NThingShot::CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun(CThing&): the base constructor sees the argument, then the
// member TrapCreated is false and the object keeps everything the base set
#include <cstdio>
struct Base { int seen; int tag; Base(int a) : seen(a), tag(0x55) { } };
struct Derived : Base { bool m; Derived(int a) : Base(a), m(false) { } };
int main()
{
    int fails = 0;
    Derived d(7);
    if (d.seen != 7 || d.tag != 0x55) fails |= 1;
    double diff = (double)d.m - (double)(false);
    if (diff > 1e-6 || diff < -1e-6) fails |= 2;
    if (fails) { std::printf("004dca62_TEST FAIL %d\n", fails); return 1; }
    std::printf("004dca62_TEST PASS\n");
    return 0;
}