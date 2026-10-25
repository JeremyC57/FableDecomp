// Behaviour test for 00c96430: libvorbis vorbis_book_decode -- a decoded packed entry maps through dec_index; a negative status passes through
#include <cstdio>
#include <cstring>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct codebook { long dim, entries, used_entries; const void* c; float* valuelist; unsigned int* codelist; int* dec_index; };
static long g_next; static codebook* g_book; static oggpack_buffer* g_b;
static long decode_packed_entry_number(codebook* book, oggpack_buffer* b) { g_book = book; g_b = b; return g_next; }
static long vorbis_book_decode(codebook* book, oggpack_buffer* b)
{
    long packed_entry = decode_packed_entry_number(book, b);
    if (packed_entry >= 0) return book->dec_index[packed_entry];
    return packed_entry;
}
int main()
{
    int idx[4] = { 7, 3, 9, 0 };
    codebook c; std::memset(&c, 0, sizeof c); c.dec_index = idx; oggpack_buffer b;
    g_next = 2; int ok = vorbis_book_decode(&c, &b) == 9 && g_book == &c && g_b == &b;
    g_next = 0; ok &= vorbis_book_decode(&c, &b) == 7;
    g_next = -1; ok &= vorbis_book_decode(&c, &b) == -1;
    g_next = -3; ok &= vorbis_book_decode(&c, &b) == -3;
    std::printf(ok ? "00c96430_TEST PASS\n" : "00c96430_TEST FAIL\n");
    return ok ? 0 : 1;
}
