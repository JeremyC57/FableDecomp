// Behaviour test for 0x0059A9E0 allocator<CComponent**>::allocate: n pointer slots via malloc, 0 for n == 0.
#include <cstdio>
#include <cstdlib>
struct CComponent;
static size_t g_lastSize = 0;
static void* TrackedMalloc(size_t n) { g_lastSize = n; return std::malloc(n); }
template <class T> struct Allocator {
    T* allocate(size_t n, const void* = 0) { return n != 0 ? (T*)TrackedMalloc(n * sizeof(T)) : 0; }
};
int main()
{
    Allocator<CComponent**> a;
    CComponent*** p = a.allocate(8);
    if (p == 0 || g_lastSize != 8 * sizeof(CComponent**)) { std::printf("0059a9e0_TEST FAIL\n"); return 1; }
    p[7] = 0; std::free(p);
    g_lastSize = 99;
    if (a.allocate(0) != 0 || g_lastSize != 99) { std::printf("0059a9e0_TEST FAIL\n"); return 1; }
    std::printf("0059a9e0_TEST PASS\n");
    return 0;
}
