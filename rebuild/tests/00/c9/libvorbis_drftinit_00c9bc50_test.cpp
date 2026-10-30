// Behaviour test for 0x00C9BC50 drft_init (see src/00c9bc50.cpp): n stored, 3n trig floats and 32 split ints allocated (zeroed).
#include <cstdio>
#include <cstdlib>
struct drft_lookup { int n; float* trigcache; int* splitcache; };
static int g_setupN;
static void fdrffti(int n, float* wsave, int* ifac) { (void)wsave; (void)ifac; if (n == 1) return; g_setupN = n; }
static void drft_init(drft_lookup* l, int n)
{
    l->n = n;
    l->trigcache = (float*)std::calloc(3 * n, sizeof(*l->trigcache));
    l->splitcache = (int*)std::calloc(32, sizeof(*l->splitcache));
    fdrffti(n, l->trigcache, l->splitcache);
}
int main()
{
    drft_lookup l;
    drft_init(&l, 16);
    if (l.n != 16 || !l.trigcache || !l.splitcache || l.trigcache[47] != 0.0f || l.splitcache[31] != 0 || g_setupN != 16) { std::printf("00c9bc50_TEST FAIL\n"); return 1; }
    std::free(l.trigcache); std::free(l.splitcache);
    std::printf("00c9bc50_TEST PASS\n"); return 0;
}