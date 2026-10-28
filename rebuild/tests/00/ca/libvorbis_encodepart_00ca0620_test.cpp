// Test: _encodepart -- n/dim vectors are each matched to their best codebook entry and encoded; the encoded bit counts are summed.
#include <cstdio>
struct codebook { int dim; };
static int g_entries[8]; static int g_n = 0;
static int local_book_besterror(codebook* b, float* a) { return (int)a[0] + b->dim; }
static int vorbis_book_encode(codebook*, int entry, void*) { g_entries[g_n++] = entry; return entry + 1; }
static int _encodepart(void* opb, float* vec, int n, codebook* book, int*) {
    int i, bits = 0; int dim = book->dim; int step = n / dim;
    for (i = 0; i < step; i++) { int entry = local_book_besterror(book, vec + i * dim); bits += vorbis_book_encode(book, entry, opb); }
    return bits; }
int main()
{
    float vec[7] = { 1, 9, 3, 9, 5, 9, 7 }; codebook book; book.dim = 2;
    int bits = _encodepart(0, vec, 7, &book, 0);
    bool ok = g_n == 3 && g_entries[0] == 3 && g_entries[1] == 5 && g_entries[2] == 7 && bits == 18;
    std::printf(ok ? "00ca0620_TEST PASS\n" : "00ca0620_TEST FAIL\n");
    return ok ? 0 : 1;
}