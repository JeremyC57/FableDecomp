// Behaviour test for 00c954a0: libvorbis vorbis_book_init_encode -- book zeroed, entries/dim copied, codelist/valuelist from the two builders, returns 0
#include <cstdio>
#include <cstring>
typedef unsigned int ogg_uint32_t;
struct static_codebook { long dim, entries; long* lengthlist; int maptype; long q_min, q_delta; int q_quant, q_sequencep; long* quantlist; void *nt, *tt, *pt; int allocedp; };
struct codebook { long dim, entries, used_entries; const static_codebook* c; float* valuelist; ogg_uint32_t* codelist; int* dec_index; char* dec_codelengths; ogg_uint32_t* dec_firsttable; int dec_firsttablen, dec_maxlength; };
static ogg_uint32_t g_words[4]; static float g_values[4]; static long* g_l; static long g_n, g_sc; static const static_codebook* g_b; static int g_n2; static int* g_sm = (int*)1;
static ogg_uint32_t* _make_words(long* l, long n, long sparsecount) { g_l = l; g_n = n; g_sc = sparsecount; return g_words; }
static float* _book_unquantize(const static_codebook* b, int n, int* sparsemap) { g_b = b; g_n2 = n; g_sm = sparsemap; return g_values; }
static int vorbis_book_init_encode(codebook* c, const static_codebook* s)
{
    std::memset(c, 0, sizeof(*c));
    c->c = s; c->entries = s->entries; c->used_entries = s->entries; c->dim = s->dim;
    c->codelist = _make_words(s->lengthlist, s->entries, 0);
    c->valuelist = _book_unquantize(s, s->entries, 0);
    return 0;
}
int main()
{
    long lens[4] = { 1, 2, 3, 3 };
    static_codebook s; std::memset(&s, 0, sizeof s); s.dim = 2; s.entries = 4; s.lengthlist = lens;
    codebook c; std::memset(&c, 0xCD, sizeof c);
    int ok = sizeof(codebook) == 0x2c && sizeof(static_codebook) == 0x34;
    ok &= vorbis_book_init_encode(&c, &s) == 0;
    ok &= c.c == &s && c.entries == 4 && c.used_entries == 4 && c.dim == 2 && c.codelist == g_words && c.valuelist == g_values;
    ok &= c.dec_index == 0 && c.dec_codelengths == 0 && c.dec_firsttable == 0 && c.dec_firsttablen == 0 && c.dec_maxlength == 0;
    ok &= g_l == lens && g_n == 4 && g_sc == 0 && g_b == &s && g_n2 == 4 && g_sm == 0;
    std::printf(ok ? "00c954a0_TEST PASS\n" : "00c954a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
