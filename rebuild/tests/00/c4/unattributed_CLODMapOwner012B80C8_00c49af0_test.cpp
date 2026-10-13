#include <cstdio>
struct O { virtual void v0() {} void* r; unsigned char* Buffer; ~O() { delete[] Buffer; } };
int main()
{
    O* o = new O; o->Buffer = new unsigned char[8]; delete o;

    std::printf("00c49af0_TEST PASS\n");
    return 0;
}