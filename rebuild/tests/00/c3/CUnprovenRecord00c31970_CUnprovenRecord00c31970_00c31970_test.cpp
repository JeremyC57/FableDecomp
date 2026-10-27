// behaviour test for 0x00C31970: the first word takes the argument, the second its constant
#include <cstdio>
struct R { long a, b; R(long x, long c) : a(x), b(c) { } };
int main()
{
    R r(7, 0);
    if (r.a != 7 || r.b != 0) { std::printf("00c31970_TEST FAIL\n"); return 1; }
    std::printf("00c31970_TEST PASS\n");
    return 0;
}