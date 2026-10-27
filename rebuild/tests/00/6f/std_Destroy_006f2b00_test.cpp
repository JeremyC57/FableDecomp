// behaviour test for 0x006F2B00 std::_Destroy(first, last) (model: each element's destructor runs once)
#include <cstdio>
#include <new>
static int g_dtors = 0;
struct M { ~M() { ++g_dtors; } };
struct E { int a; M m; };
int main()
{
    static double raw[8];
    E* first = (E*)raw;
    for (int i = 0; i < 3; ++i) new (first + i) E();
    for (E* p = first; p != first + 3; ++p) p->~E();
    int ok = g_dtors == 3;
    if (!ok) { std::printf("006f2b00_TEST FAIL\n"); return 1; }
    std::printf("006f2b00_TEST PASS\n");
    return 0;
}