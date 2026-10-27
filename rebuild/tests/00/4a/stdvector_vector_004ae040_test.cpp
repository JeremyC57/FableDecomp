// behaviour test for 0x004AE040: tearing the vector down destroys each element once and frees the block once
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0, g_frees = 0;
struct S { ~S() { ++g_dtors; } };
struct E { long k; S a; S b; };
static void Teardown(E* s, E* f) { for (E* p = s; p != f; ++p) p->~E(); if (s) { std::free(s); ++g_frees; } }
int main()
{
    E* v = (E*)std::malloc(3 * sizeof(E));
    Teardown(v, v + 3); Teardown(0, 0);
    if (g_dtors != 6 || g_frees != 1) { std::printf("004ae040_TEST FAIL\n"); return 1; }
    std::printf("004ae040_TEST PASS\n");
    return 0;
}