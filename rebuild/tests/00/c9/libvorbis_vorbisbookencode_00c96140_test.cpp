// Behaviour test for 00c96140: libvorbis vorbis_book_encode -- writes codelist[a] with lengthlist[a] bits and returns the length
#include <cstdio>
#include <cstring>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct static_codebook { long dim, entries; long* lengthlist; };
struct codebook { long dim, entries, used_entries; const static_codebook* c; float* valuelist; unsigned int* codelist; };
static oggpack_buffer* g_b; static unsigned long g_v; static int g_bits; static int g_calls;
static void oggpack_write(oggpack_buffer* b, unsigned long value, int bits) { g_b = b; g_v = value; g_bits = bits; ++g_calls; }
static int vorbis_book_encode(codebook* book, int a, oggpack_buffer* b)
{
    oggpack_write(b, book->codelist[a], book->c->lengthlist[a]);
    return book->c->lengthlist[a];
}
int main()
{
    long lens[3] = { 1, 2, 7 }; unsigned int words[3] = { 0x0, 0x2, 0x45 };
    static_codebook s; s.dim = 1; s.entries = 3; s.lengthlist = lens;
    codebook c; std::memset(&c, 0, sizeof c); c.c = &s; c.codelist = words; c.entries = 3;
    oggpack_buffer b;
    int ok = vorbis_book_encode(&c, 2, &b) == 7 && g_b == &b && g_v == 0x45 && g_bits == 7;
    ok &= vorbis_book_encode(&c, 1, &b) == 2 && g_v == 0x2 && g_bits == 2 && g_calls == 2;
    std::printf(ok ? "00c96140_TEST PASS\n" : "00c96140_TEST FAIL\n");
    return ok ? 0 : 1;
}
