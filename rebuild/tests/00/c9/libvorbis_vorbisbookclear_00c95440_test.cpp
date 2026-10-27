#include <cstdio>
#include <cstdlib>
#include <cstring>
// vorbis_book_clear: free the five owned arrays (null-tested), then wipe the whole record.
static int g_frees = 0;
static void counting_free(void* p) { ++g_frees; std::free(p); }
struct codebook {
    int dim; int entries; int used_entries; const void* c;
    float* valuelist; unsigned int* codelist; int* dec_index; char* dec_codelengths; unsigned int* dec_firsttable;
    int dec_firsttablen; int dec_maxlength;
};
static void vorbis_book_clear(codebook* b)
{
    if (b->valuelist) counting_free(b->valuelist);
    if (b->codelist) counting_free(b->codelist);
    if (b->dec_index) counting_free(b->dec_index);
    if (b->dec_codelengths) counting_free(b->dec_codelengths);
    if (b->dec_firsttable) counting_free(b->dec_firsttable);
    std::memset(b, 0, sizeof(*b));
}
int main()
{
    codebook b;
    b.dim = 2; b.entries = 8; b.used_entries = 7; b.c = &b;
    b.valuelist = (float*)std::malloc(16); b.codelist = 0; b.dec_index = (int*)std::malloc(8);
    b.dec_codelengths = (char*)std::malloc(4); b.dec_firsttable = 0; b.dec_firsttablen = 3; b.dec_maxlength = 9;
    vorbis_book_clear(&b);
    if (g_frees != 3 || b.dim != 0 || b.c != 0 || b.valuelist != 0 || b.dec_maxlength != 0) { std::printf("00c95440_TEST FAIL\n"); return 1; }
    std::printf("00c95440_TEST PASS\n");
    return 0;
}