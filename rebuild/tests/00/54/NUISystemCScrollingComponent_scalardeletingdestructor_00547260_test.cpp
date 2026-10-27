// behaviour test for 0x00547260: deleting through the base runs the derived and base destructors and frees once
#include <cstdio>
#include <new>
static int g_base = 0, g_derived = 0;
struct B { virtual ~B() { ++g_base; } };
struct D : B { ~D() { ++g_derived; } };
int main()
{
    B* p = new D; delete p;
    D d; d.~D(); new (&d) D;
    if (g_base != 2 || g_derived != 2) { std::printf("00547260_TEST FAIL\n"); return 1; }
    std::printf("00547260_TEST PASS\n");
    return 0;
}