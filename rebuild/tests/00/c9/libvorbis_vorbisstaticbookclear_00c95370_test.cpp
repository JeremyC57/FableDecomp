// Behaviour test for 0x00C95370 libvorbis vorbis_staticbook_clear (copy of the model + logic, counting frees).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct encode_aux_nearestmatch { long* ptr0; long* ptr1; long* p; long* q; long aux; long alloc; };
struct encode_aux_threshmatch { float* quantthresh; long* quantmap; int quantvals; int threshvals; };
struct static_codebook {
    long dim, entries; long* lengthlist; int maptype; long q_min, q_delta; int q_quant, q_sequencep; long* quantlist;
    encode_aux_nearestmatch* nearest_tree; encode_aux_threshmatch* thresh_tree; void* pigeon_tree; int allocedp;
};
static int g_frees;
static void count_free(void* p) { g_frees++; std::free(p); }
static void staticbook_clear(static_codebook* b)
{
    if (b->allocedp) {
        if (b->quantlist) count_free(b->quantlist);
        if (b->lengthlist) count_free(b->lengthlist);
        if (b->nearest_tree) {
            count_free(b->nearest_tree->ptr0); count_free(b->nearest_tree->ptr1);
            count_free(b->nearest_tree->p); count_free(b->nearest_tree->q);
            std::memset(b->nearest_tree, 0, sizeof(*b->nearest_tree));
            count_free(b->nearest_tree);
        }
        if (b->thresh_tree) {
            count_free(b->thresh_tree->quantthresh); count_free(b->thresh_tree->quantmap);
            std::memset(b->thresh_tree, 0, sizeof(*b->thresh_tree));
            count_free(b->thresh_tree);
        }
        std::memset(b, 0, sizeof(*b));
    }
}
int main()
{
    int ok = 1;
    static_codebook b;
    std::memset(&b, 0, sizeof(b));
    b.dim = 2; b.entries = 81;
    staticbook_clear(&b);                                    /* not allocated: untouched */
    if (b.dim != 2 || b.entries != 81 || g_frees != 0) ok = 0;
    b.allocedp = 1;
    b.quantlist = (long*)std::malloc(8); b.lengthlist = (long*)std::malloc(8);
    b.nearest_tree = (encode_aux_nearestmatch*)std::calloc(1, sizeof(encode_aux_nearestmatch));
    b.nearest_tree->ptr0 = (long*)std::malloc(4); b.nearest_tree->q = (long*)std::malloc(4);
    b.thresh_tree = (encode_aux_threshmatch*)std::calloc(1, sizeof(encode_aux_threshmatch));
    b.thresh_tree->quantmap = (long*)std::malloc(4);
    staticbook_clear(&b);
    if (g_frees != 2 + 5 + 3) ok = 0;                        /* tree members are freed unconditionally */
    if (b.dim || b.entries || b.quantlist || b.nearest_tree || b.thresh_tree || b.allocedp) ok = 0;
    if (!ok) { std::printf("00c95370_TEST FAIL\n"); return 1; }
    std::printf("00c95370_TEST PASS\n");
    return 0;
}