#include <cstdio>
#include <cstdlib>
static int g_d = 0; struct S { ~S() { ++g_d; } };
struct X { S s; void* b; ~X() { if (b) std::free(b); } };
int main()
{
    { X x; x.b = std::malloc(4); } if (g_d != 1) { std::printf("009d3210_TEST FAIL\n"); return 1; }

    std::printf("009d3210_TEST PASS\n");
    return 0;
}
