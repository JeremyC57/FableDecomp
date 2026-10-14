#include <cstdio>
struct X { unsigned char p[4]; void** a; void* G(long index) { return a[index]; } };
int main()
{
    int i; void* v[2] = { 0, &i }; X x; x.a = v; if (x.G(1) != &i) { std::printf("00850800_TEST FAIL\n"); return 1; }

    std::printf("00850800_TEST PASS\n");
    return 0;
}