#include <cstdio>
struct O { void** t; void* M(void* e) { return e; } void* F(long i) { return M(t[i]); } };
int main()
{
    int a, b; void* arr[2] = { &a, &b }; O o; o.t = arr; if (o.F(1) != &b) { std::printf("00c42b80_TEST FAIL\n"); return 1; }

    std::printf("00c42b80_TEST PASS\n");
    return 0;
}
