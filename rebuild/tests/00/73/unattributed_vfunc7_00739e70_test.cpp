#include <cstdio>
struct X { float G(long, long, long) { return 0.0f; } };
int main()
{
    X x; if (x.G(0, 0, 0) != 0.0f) { std::printf("00739e70_TEST FAIL\n"); return 1; }

    std::printf("00739e70_TEST PASS\n");
    return 0;
}