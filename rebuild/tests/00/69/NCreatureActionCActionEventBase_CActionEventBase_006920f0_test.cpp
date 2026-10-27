// behaviour test for 0x006920F0: the constructor sets the one member it owns
#include <cstdio>
struct Base { void* vt; Base() : vt(0) { } };
struct X : Base { long member; X() : member(0) { } };
int main()
{
    X x;
    if (x.member != (long)0) { std::printf("006920f0_TEST FAIL\n"); return 1; }
    std::printf("006920f0_TEST PASS\n");
    return 0;
}