// Behaviour test for 0x00C96700 libvorbis vorbis_book_decodev_set (copy of the model + logic; the entry decoder is a scripted stand-in).
#include <cstdio>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct codebook { long dim; long entries; long used_entries; const void* c; float* valuelist; };
static const int* g_script; static int g_pos, g_len;
static long decode_entry(codebook*, oggpack_buffer*) { return g_pos < g_len ? g_script[g_pos++] : -1; }
static long decodev_set(codebook* book, float* a, oggpack_buffer* b, int n)
{
    int i, j, entry; float* t;
    for (i = 0; i < n;) {
        entry = decode_entry(book, b);
        if (entry == -1) return -1;
        t = book->valuelist + entry * book->dim;
        for (j = 0; j < book->dim;) a[i++] = t[j++];
    }
    return 0;
}
int main()
{
    float vals[3 * 2] = { 10, 11, 20, 21, 30, 31 };
    codebook book = { 2, 3, 3, 0, vals };
    oggpack_buffer ob = { 0 };
    int script[3] = { 2, 0, 1 };
    float a[6] = { 0 };
    g_script = script; g_pos = 0; g_len = 3;
    int ok = decodev_set(&book, a, &ob, 6) == 0 && a[0] == 30 && a[1] == 31 && a[2] == 10 && a[3] == 11 && a[4] == 20 && a[5] == 21;
    g_pos = 0; g_len = 1;
    if (decodev_set(&book, a, &ob, 6) != -1) ok = 0;         /* runs out after one entry */
    if (!ok) { std::printf("00c96700_TEST FAIL\n"); return 1; }
    std::printf("00c96700_TEST PASS\n");
    return 0;
}