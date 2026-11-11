#include <cstdio>
struct X { unsigned char p[4]; short v; short G() { return v; } };
int main()
{
    X x; x.v = -3; if (x.G() != -3) { std::printf("0042d506_TEST FAIL\n"); return 1; }

    std::printf("0042d506_TEST PASS\n");
    return 0;
}