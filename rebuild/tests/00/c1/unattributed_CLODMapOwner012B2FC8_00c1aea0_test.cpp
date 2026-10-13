#include <cstdio>
struct O { virtual void v0() {} void* r; unsigned char* Buffer; ~O() { delete Buffer; } };
int main()
{
    O* o = new O; o->Buffer = new unsigned char; delete o;

    std::printf("00c1aea0_TEST PASS\n");
    return 0;
}