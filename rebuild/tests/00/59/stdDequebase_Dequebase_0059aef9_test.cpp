// behaviour test for 0x0059AEF9: the base starts with null iterators and map before sizing the map for n elements
#include <cstdio>
static unsigned int g_n = 12345; static int g_zero_at_init = 0;
struct It { void* c; void* f; void* l; void** n; It() : c(0), f(0), l(0), n(0) { } };
struct B {
    It s, e; void** map; unsigned int size;
    B(unsigned int n) : map(0), size(0) { Init(n); }
    void Init(unsigned int n) { g_n = n; g_zero_at_init = !s.c && !e.n && !map && !size; }
};
int main()
{
    B b(7);
    if (g_n != 7 || !g_zero_at_init) { std::printf("0059aef9_TEST FAIL\n"); return 1; }
    std::printf("0059aef9_TEST PASS\n");
    return 0;
}