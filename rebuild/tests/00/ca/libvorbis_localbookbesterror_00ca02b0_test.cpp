// Behaviour test for libvorbis res0.c local_book_besterror (0x00CA02B0): quantise each scalar through the threshold tree
// (last scalar first) into a lattice entry; if that entry has no code length, brute-force the nearest coded entry; then
// subtract the chosen entry's values from the input vector and return the entry.
#include <cstdio>
#include <cmath>
struct encode_aux_threshmatch { float* quantthresh; long* quantmap; int quantvals; int threshvals; };
struct static_codebook { long* lengthlist; encode_aux_threshmatch* thresh_tree; };
struct codebook { long dim; long entries; const static_codebook* c; float* valuelist; };
static int local_book_besterror(codebook* book, float* a)
{
    int dim = book->dim, i, k, o;
    int best = 0;
    encode_aux_threshmatch* tt = book->c->thresh_tree;
    for (k = 0, o = dim; k < dim; ++k) {
        float val = a[--o];
        i = tt->threshvals >> 1;
        if (val < tt->quantthresh[i]) {
            if (val < tt->quantthresh[i - 1]) {
                for (--i; i > 0; --i)
                    if (val >= tt->quantthresh[i - 1]) break;
            }
        } else {
            for (++i; i < tt->threshvals - 1; ++i)
                if (val < tt->quantthresh[i]) break;
        }
        best = (best * tt->quantvals) + tt->quantmap[i];
    }
    if (book->c->lengthlist[best] <= 0) {
        const static_codebook* c = book->c;
        int i, j;
        float bestf = 0.f;
        int dim = book->dim;
        float* e = book->valuelist;
        best = -1;
        for (i = 0; i < book->entries; i++) {
            if (c->lengthlist[i] > 0) {
                float thisf = 0.f;
                for (j = 0; j < dim; j++) { float val = (e[j] - a[j]); thisf += val * val; }
                if (best == -1 || thisf < bestf) { bestf = thisf; best = i; }
            }
            e += dim;
        }
    }
    {
        float* ptr = book->valuelist + best * dim;
        for (i = 0; i < dim; i++) *a++ -= *ptr++;
    }
    return best;
}
static int fail(const char* why) { std::printf("00ca02b0_TEST FAIL %s\n", why); return 1; }
int main()
{
    // quantvals 3 (values -1, 0, 1), thresholds between them: -0.5, 0.5; quantmap identity.
    float thresh[2] = { -0.5f, 0.5f };
    long qmap[3] = { 0, 1, 2 };
    encode_aux_threshmatch tt; tt.quantthresh = thresh; tt.quantmap = qmap; tt.quantvals = 3; tt.threshvals = 3;
    long lengths[9]; float values[18];
    for (int e = 0; e < 9; ++e) { lengths[e] = 1; values[e * 2] = (float)(e % 3 - 1); values[e * 2 + 1] = (float)(e / 3 - 1); }   // entry = q(a[1]) * 3 + q(a[0])
    static_codebook sc; sc.lengthlist = lengths; sc.thresh_tree = &tt;
    codebook book; book.dim = 2; book.entries = 9; book.c = &sc; book.valuelist = values;
    float a[2] = { 0.9f, -0.8f };                 // q = (2, 0) -> entry 0*3 + 2 = 2 (the last scalar is the most significant)
    int r = local_book_besterror(&book, a);
    if (r != 2) return fail("lattice entry");
    if (std::fabs(a[0] - (-0.1f)) > 1e-5f || std::fabs(a[1] - 0.2f) > 1e-5f) return fail("lattice residue");
    lengths[2] = 0;                               // now entry 2 is uncoded: nearest coded is 5 (1,0) at 0.65 over 1 (0,-1) at 0.85
    float b[2] = { 0.9f, -0.8f };
    r = local_book_besterror(&book, b);
    if (r != 5) return fail("brute-force entry");
    if (std::fabs(b[0] - (-0.1f)) > 1e-5f || std::fabs(b[1] - (-0.8f)) > 1e-5f) return fail("brute-force residue");
    float c[2] = { 0.0f, 0.4f };                  // (0, 0) -> entry 4
    if (local_book_besterror(&book, c) != 4) return fail("centre entry");
    std::printf("00ca02b0_TEST PASS\n");
    return 0;
}