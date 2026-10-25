// Behaviour test for 0x00C9BC90 libvorbis drft_clear (copy of the model + logic, counting frees).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct drft_lookup { int n; float* trigcache; int* splitcache; };
static int g_frees;
static void count_free(void* p) { g_frees++; std::free(p); }
static void drft_clear_t(drft_lookup* l)
{
    if (l) {
        if (l->trigcache) count_free(l->trigcache);
        if (l->splitcache) count_free(l->splitcache);
        std::memset(l, 0, sizeof(*l));
    }
}
int main()
{
    drft_lookup l; l.n = 64; l.trigcache = (float*)std::malloc(64); l.splitcache = (int*)std::malloc(64);
    drft_clear_t(&l);
    int ok = g_frees == 2 && l.n == 0 && l.trigcache == 0 && l.splitcache == 0;
    l.n = 8; l.splitcache = (int*)std::malloc(8);
    drft_clear_t(&l);
    if (g_frees != 3 || l.n) ok = 0;
    drft_clear_t(0);
    if (!ok) { std::printf("00c9bc90_TEST FAIL\n"); return 1; }
    std::printf("00c9bc90_TEST PASS\n");
    return 0;
}