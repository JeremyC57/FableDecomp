// Behaviour test for 00c95420: libvorbis vorbis_staticbook_destroy -- an allocated book is cleared then freed, a static one is untouched
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct static_codebook { long dim, entries; long* lengthlist; int maptype; long q_min, q_delta; int q_quant, q_sequencep; long* quantlist; void *nt, *tt, *pt; int allocedp; };
static int g_cleared, g_freed; static void* g_lastFreed;
static void vorbis_staticbook_clear(static_codebook* b) { ++g_cleared; std::memset(b, 0, sizeof *b); }
static void my_free(void* p) { ++g_freed; g_lastFreed = p; std::free(p); }
static void vorbis_staticbook_destroy(static_codebook* b)
{
    if (b->allocedp) { vorbis_staticbook_clear(b); my_free(b); }
}
int main()
{
    static_codebook st; std::memset(&st, 0, sizeof st); st.entries = 5;
    vorbis_staticbook_destroy(&st);
    int ok = g_cleared == 0 && g_freed == 0 && st.entries == 5;
    static_codebook* h = (static_codebook*)std::malloc(sizeof *h); std::memset(h, 0, sizeof *h); h->allocedp = 1;
    vorbis_staticbook_destroy(h);
    ok &= g_cleared == 1 && g_freed == 1 && g_lastFreed == h && sizeof(static_codebook) == 0x34;
    std::printf(ok ? "00c95420_TEST PASS\n" : "00c95420_TEST FAIL\n");
    return ok ? 0 : 1;
}
