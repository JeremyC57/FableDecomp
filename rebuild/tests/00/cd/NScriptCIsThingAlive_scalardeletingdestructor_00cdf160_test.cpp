// behaviour test for 0x00CDF160: deleting through the base runs the member's destructor once and frees the object
#include <cstdio>
static int g_member = 0;
struct M { ~M() { ++g_member; } };
struct B { virtual ~B() { } };
struct D : B { M m; };
int main()
{
    B* p = new D; delete p;
    if (g_member != 1) { std::printf("00cdf160_TEST FAIL\n"); return 1; }
    std::printf("00cdf160_TEST PASS\n");
    return 0;
}