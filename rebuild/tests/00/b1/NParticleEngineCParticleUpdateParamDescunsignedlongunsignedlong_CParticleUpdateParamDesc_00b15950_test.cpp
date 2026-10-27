// behaviour test for 0x00B15950: the constructor sets the one member it owns
#include <cstdio>
struct Base { void* vt; Base() : vt(0) { } };
struct X : Base { long member; X() : member(0) { } };
int main()
{
    X x;
    if (x.member != (long)0) { std::printf("00b15950_TEST FAIL\n"); return 1; }
    std::printf("00b15950_TEST PASS\n");
    return 0;
}