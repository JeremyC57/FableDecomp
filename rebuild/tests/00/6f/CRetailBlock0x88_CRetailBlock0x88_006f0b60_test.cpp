#include <cstdio>
#include <cstdlib>
struct A {}; struct X { unsigned char* b; X(A) : b(0) { b = (unsigned char*)std::malloc(0x88); } };
int main()
{
    A al; X x(al); if (!x.b) { std::printf("006f0b60_TEST FAIL\n"); return 1; }
    std::free(x.b);

    std::printf("006f0b60_TEST PASS\n");
    return 0;
}