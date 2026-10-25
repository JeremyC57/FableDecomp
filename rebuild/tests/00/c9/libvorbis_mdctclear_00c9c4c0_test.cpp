// Behaviour test for 0x00C9C4C0 libvorbis mdct_clear (copy of the model + logic, counting frees).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct mdct_lookup { int n; int log2n; float* trig; int* bitrev; float scale; };
static int g_frees;
static void count_free(void* p) { g_frees++; std::free(p); }
static void mdct_clear_t(mdct_lookup* l)
{
    if (l) {
        if (l->trig) count_free(l->trig);
        if (l->bitrev) count_free(l->bitrev);
        std::memset(l, 0, sizeof(*l));
    }
}
int main()
{
    mdct_lookup l; l.n = 256; l.log2n = 8; l.trig = (float*)std::malloc(64); l.bitrev = 0; l.scale = 0.5f;
    mdct_clear_t(&l);
    int ok = g_frees == 1 && l.n == 0 && l.log2n == 0 && l.trig == 0 && l.scale == 0.f;
    mdct_clear_t(0);
    if (g_frees != 1) ok = 0;
    if (!ok) { std::printf("00c9c4c0_TEST FAIL\n"); return 1; }
    std::printf("00c9c4c0_TEST PASS\n");
    return 0;
}