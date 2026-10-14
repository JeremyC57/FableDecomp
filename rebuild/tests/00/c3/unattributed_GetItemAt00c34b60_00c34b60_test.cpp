#include <cstdio>
struct X { unsigned char p[4]; void** a; void* G(long index, long unused1) { return a[index]; } };
int main()
{
    int i; void* v[2] = { 0, &i }; X x; x.a = v; if (x.G(1, 0) != &i) { std::printf("00c34b60_TEST FAIL\n"); return 1; }

    std::printf("00c34b60_TEST PASS\n");
    return 0;
}