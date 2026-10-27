#include <cstdio>
#include <vector>
// Behaviour model: the wrapper forwards exactly element [index] of the wrapped vector.
struct E { long a; void* b; };
static const E* g_seen;
static void traits_transfer(E& v) { g_seen = &v; }
struct W { std::vector<E>* v; void Transfer(long i) { traits_transfer((*v)[i]); } };
int main()
{
    std::vector<E> v(4);
    W w = { &v };
    w.Transfer(2);
    if (g_seen != &v[2]) { std::printf("006c78b0_TEST FAIL\n"); return 1; }
    w.Transfer(0);
    if (g_seen != &v[0]) { std::printf("006c78b0_TEST FAIL\n"); return 1; }
    std::printf("006c78b0_TEST PASS\n");
    return 0;
}