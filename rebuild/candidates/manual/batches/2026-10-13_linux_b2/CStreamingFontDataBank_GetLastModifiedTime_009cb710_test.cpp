#include <cstdio>

static long g_sum = 0;
struct CCharString;
struct CDateAndTime { long v[7]; CDateAndTime(long a, long b, long c, long d, long e, long f, long g) { v[0]=a; v[1]=b; v[2]=c; v[3]=d; v[4]=e; v[5]=f; v[6]=g; g_sum = a+b+c+d+e+f+g; } };
struct Bank { CDateAndTime Get(CCharString*) { return CDateAndTime(1, 1, 1, 1, 1, 0, 0); } };
int main()
{
    Bank b; CDateAndTime t = b.Get(0);
    if (g_sum != 5 || t.v[5] != 0 || t.v[4] != 1) { std::printf("009cb710_TEST FAIL\n"); return 1; }
    std::printf("009cb710_TEST PASS\n");
    return 0;
}
