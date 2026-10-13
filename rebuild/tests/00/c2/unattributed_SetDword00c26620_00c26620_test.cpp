#include <cstdio>
struct X { unsigned char p[4]; long v; void Set(long a) { v = a; } };
int main()
{
    X x; x.Set(3); if (x.v != 3) { std::printf("00c26620_TEST FAIL\n"); return 1; }

    std::printf("00c26620_TEST PASS\n");
    return 0;
}