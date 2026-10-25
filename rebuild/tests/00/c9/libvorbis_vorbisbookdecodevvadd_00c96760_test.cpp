// Behaviour test for 0x00C96760 libvorbis vorbis_book_decodevv_add (copy of the model + logic; scripted entry decoder).
#include <cstdio>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct codebook { long dim; long entries; long used_entries; const void* c; float* valuelist; };
static const int* g_script; static int g_pos, g_len;
static long decode_entry(codebook*, oggpack_buffer*) { return g_pos < g_len ? g_script[g_pos++] : -1; }
static long decodevv_add(codebook* book, float** a, long offset, int ch, oggpack_buffer* b, int n)
{
    long i, j, entry; int chptr = 0;
    for (i = offset / ch; i < (offset + n) / ch;) {
        entry = decode_entry(book, b);
        if (entry == -1) return -1;
        {
            const float* t = book->valuelist + entry * book->dim;
            for (j = 0; j < book->dim; j++) {
                a[chptr++][i] += t[j];
                if (chptr == ch) { chptr = 0; i++; }
            }
        }
    }
    return 0;
}
int main()
{
    float vals[2 * 3] = { 1, 2, 3, 10, 20, 30 };
    codebook book = { 3, 2, 2, 0, vals };
    oggpack_buffer ob = { 0 };
    float c0[4] = { 100, 100, 100, 100 }, c1[4] = { 100, 100, 100, 100 };
    float* a[2] = { c0, c1 };
    int script[2] = { 0, 1 };
    g_script = script; g_pos = 0; g_len = 2;
    /* offset 2, 2 channels, 6 values: positions 1..3 interleaved c0,c1,c0,c1,c0,c1 */
    int ok = decodevv_add(&book, a, 2, 2, &ob, 6) == 0 &&
             c0[0] == 100 && c0[1] == 101 && c1[1] == 102 && c0[2] == 103 && c1[2] == 110 && c0[3] == 120 && c1[3] == 130;
    g_pos = 0; g_len = 0;
    if (decodevv_add(&book, a, 0, 2, &ob, 2) != -1) ok = 0;
    if (!ok) { std::printf("00c96760_TEST FAIL\n"); return 1; }
    std::printf("00c96760_TEST PASS\n");
    return 0;
}