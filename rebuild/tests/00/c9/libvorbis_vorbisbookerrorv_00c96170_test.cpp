// Behaviour test for 0x00C96170 libvorbis vorbis_book_errorv (copy of the model + logic; _best is a brute-force stand-in).
#include <cstdio>
struct codebook { long dim; long entries; long used_entries; const void* c; float* valuelist; };
static int g_step;
static int best_t(codebook* book, float* a, int step)
{
    int e, k, best = 0; float bestd = 1e30f;
    g_step = step;
    for (e = 0; e < book->entries; e++) {
        float d = 0;
        for (k = 0; k < book->dim; k++) { float t = a[k * step] - book->valuelist[e * book->dim + k]; d += t * t; }
        if (d < bestd) { bestd = d; best = e; }
    }
    return best;
}
static int book_errorv(codebook* book, float* a)
{
    int dim = book->dim, k;
    int best = best_t(book, a, 1);
    for (k = 0; k < dim; k++) a[k] = (book->valuelist + best * dim)[k];
    return best;
}
int main()
{
    float vals[3 * 5] = { 0, 0, 0, 0, 0,   1, 2, 3, 4, 5,   -1, -1, -1, -1, -1 };
    codebook book = { 5, 3, 3, 0, vals };
    float a[5] = { 0.9f, 2.2f, 2.8f, 4.1f, 5.3f };
    int ok = book_errorv(&book, a) == 1 && g_step == 1 && a[0] == 1 && a[1] == 2 && a[2] == 3 && a[3] == 4 && a[4] == 5;
    float b[5] = { -0.8f, -1.2f, -1, -0.9f, -1.1f };
    if (book_errorv(&book, b) != 2 || b[0] != -1 || b[4] != -1) ok = 0;
    if (!ok) { std::printf("00c96170_TEST FAIL\n"); return 1; }
    std::printf("00c96170_TEST PASS\n");
    return 0;
}