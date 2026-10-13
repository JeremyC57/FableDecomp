#include <cstdio>
#include <cstdlib>
struct A {}; struct X { unsigned char* b; X(A) : b(0) { b = (unsigned char*)std::malloc(0x18); } };
int main()
{
    A al; X x(al); if (!x.b) { std::printf("00a54160_TEST FAIL\n"); return 1; }
    std::free(x.b);

    std::printf("00a54160_TEST PASS\n");
    return 0;
}