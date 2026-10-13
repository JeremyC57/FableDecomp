#include <cstdio>
struct O { virtual void v0() {} void* r; unsigned char* Buffer; ~O() { delete Buffer; } };
int main()
{
    O* o = new O; o->Buffer = new unsigned char; delete o;

    std::printf("00c4bb00_TEST PASS\n");
    return 0;
}
