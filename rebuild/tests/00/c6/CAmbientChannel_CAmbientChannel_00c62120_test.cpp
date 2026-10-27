// behaviour test for 0x00C62120: the first word takes the argument, the second its constant
#include <cstdio>
struct R { long a, b; R(long x, long c) : a(x), b(c) { } };
int main()
{
    R r(7, -1);
    if (r.a != 7 || r.b != -1) { std::printf("00c62120_TEST FAIL\n"); return 1; }
    std::printf("00c62120_TEST PASS\n");
    return 0;
}