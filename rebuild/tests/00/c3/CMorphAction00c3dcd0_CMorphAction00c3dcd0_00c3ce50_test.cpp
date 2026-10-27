// behaviour test for 0x00C3CE50: the constructor sets the one member it owns
#include <cstdio>
struct Base { void* vt; Base() : vt(0) { } };
struct X : Base { long member; X() : member(0x80000000L) { } };
int main()
{
    X x;
    if (x.member != (long)0x80000000L) { std::printf("00c3ce50_TEST FAIL\n"); return 1; }
    std::printf("00c3ce50_TEST PASS\n");
    return 0;
}