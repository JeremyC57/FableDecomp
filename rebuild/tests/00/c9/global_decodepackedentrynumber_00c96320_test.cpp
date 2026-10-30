#include <cstdio>
// decode_packed_entry_number(codebook* book, oggpack_buffer* b) -- libvorbis 1.0.1 codebook.c: decode the next codeword through the first-level table, else bisect the sorted codeword list; -1 on a bad or truncated codeword
// Leaf: _decode_packed_entry_number@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C96320, 268 bytes, `push b ; push book ; call`, callee pops (ret 8): the bound
//   `_decode_packed_entry_number@8` (reloc_bindings, landed callers vorbis_book_decode 0x00C96430,
//   vorbis_book_decodev_set 0x00C96700). Xiph libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240) lib/codebook.c,
//   where it is the STIN helper the six decode entry points call (0x00C96435 .. 0x00C96796).
// Callees: oggpack_look 0x00C913A0 and oggpack_adv 0x00C91530 (landed libogg, `__fastcall`), bitreverse 0x00C962B0
//   (the codebook.c static: lok in eax, result in eax -- a TU-local private convention, so its body sits in this TU).
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;
typedef struct static_codebook static_codebook;
typedef unsigned int ogg_uint32_t;
typedef struct codebook {                          // libvorbis 1.0.1 codebook.h, 0x2c bytes
    long                   dim;                    // +0x00
    long                   entries;                // +0x04
    long                   used_entries;           // +0x08
    const static_codebook* c;                      // +0x0c
    float*                 valuelist;              // +0x10
    ogg_uint32_t*          codelist;               // +0x14
    int*                   dec_index;              // +0x18
    char*                  dec_codelengths;        // +0x1c
    ogg_uint32_t*          dec_firsttable;         // +0x20
    int                    dec_firsttablen;        // +0x24
    int                    dec_maxlength;          // +0x28
} codebook;

// test bit reader: LSb-first over the byte buffer, -1 past the end (libogg semantics)
static long oggpack_look(oggpack_buffer* b, int bits)
{
    long pos = b->endbyte * 8 + b->endbit;
    if (pos + bits > b->storage * 8) return -1;
    unsigned long v = 0;
    for (int i = 0; i < bits; ++i, ++pos)
        v |= (unsigned long)((b->buffer[pos >> 3] >> (pos & 7)) & 1) << i;
    return (long)v;
}
static int g_advTotal = 0;
static void oggpack_adv(oggpack_buffer* b, int bits)
{
    g_advTotal += bits;
    long pos = b->endbyte * 8 + b->endbit + bits;
    b->endbyte = pos >> 3; b->endbit = (int)(pos & 7);
}

static ogg_uint32_t bitreverse(ogg_uint32_t x)     // 0x00C962B0
{
    x=    ((x>>16)&0x0000ffff) | ((x<<16)&0xffff0000);
    x=    ((x>> 8)&0x00ff00ff) | ((x<< 8)&0xff00ff00);
    x=    ((x>> 4)&0x0f0f0f0f) | ((x<< 4)&0xf0f0f0f0);
    x=    ((x>> 2)&0x33333333) | ((x<< 2)&0xcccccccc);
    return((x>> 1)&0x55555555) | ((x<< 1)&0xaaaaaaaa);
}


static long decode_packed_entry_number(codebook* book, oggpack_buffer* b)
{
    int  read=book->dec_maxlength;
    long lo,hi;
    long lok = oggpack_look(b,book->dec_firsttablen);

    if (lok >= 0) {
        long entry = book->dec_firsttable[lok];
        if(entry&0x80000000UL){
            lo=(entry>>15)&0x7fff;
            hi=book->used_entries-(entry&0x7fff);
        }else{
            oggpack_adv(b, book->dec_codelengths[entry-1]);
            return(entry-1);
        }
    }else{
        lo=0;
        hi=book->used_entries;
    }

    lok = oggpack_look(b, read);

    while(lok<0 && read>1)
        lok = oggpack_look(b, --read);
    if(lok<0)return -1;

    /* bisect search for the codeword in the ordered list */
    {
        ogg_uint32_t testword=bitreverse((ogg_uint32_t)lok);

        while(hi-lo>1){
            long p=(hi-lo)>>1;
            long test=book->codelist[lo+p]>testword;
            lo+=p&(test-1);
            hi-=p&(-test);
        }

        if(book->dec_codelengths[lo]<=read){
            oggpack_adv(b, book->dec_codelengths[lo]);
            return(lo);
        }
    }

    oggpack_adv(b, read);
    return(-1);
}

int main()
{
    // 4 codewords sorted by bit-reversed value (MSb-first codes 0, 10, 110, 111 -> reversed into the top bits)
    static ogg_uint32_t codelist[4] = { 0x00000000u, 0x80000000u, 0xC0000000u, 0xE0000000u };
    static char lengths[4] = { 1, 2, 3, 3 };
    static ogg_uint32_t first[2];
    codebook book = { 0 };
    book.used_entries = 4; book.codelist = codelist; book.dec_codelengths = lengths;
    book.dec_firsttable = first; book.dec_firsttablen = 1; book.dec_maxlength = 3;
    // first-level table: bit 0 -> entry 1 (codeword "0", direct); bit 1 -> escape, lo 1, hi 4 - 0 = 4
    first[0] = 1; first[1] = 0x80000000u | (1u << 15) | 0u;
    // bits LSb-first: "0" | "1 0" | "1 1 1" | "1 1 0" -> 0, 1,0, 1,1,1, 1,1,0
    unsigned char data[2] = { 0, 0 };
    int seq[9] = { 0, 1, 0, 1, 1, 1, 1, 1, 0 };
    for (int i = 0; i < 9; ++i) if (seq[i]) data[i >> 3] |= (unsigned char)(1 << (i & 7));
    oggpack_buffer b = { 0, 0, data, data, 2 };
    long e0 = decode_packed_entry_number(&book, &b);
    long e1 = decode_packed_entry_number(&book, &b);
    long e2 = decode_packed_entry_number(&book, &b);
    long e3 = decode_packed_entry_number(&book, &b);
    if (e0 != 0 || e1 != 1 || e2 != 3 || e3 != 2 || g_advTotal != 9)
    { std::printf("00c96320_TEST FAIL\n"); return 1; }
    // truncated stream: one bit left, a "1" prefix needs more -> the read shrinks to 1, codeword too long -> -1
    unsigned char one[1] = { 0x80 };
    oggpack_buffer t = { 0, 7, one, one, 1 };
    g_advTotal = 0;
    long bad = decode_packed_entry_number(&book, &t);
    if (bad != -1 || g_advTotal != 1)
    { std::printf("00c96320_TEST FAIL\n"); return 1; }
    // no bits at all -> -1 without advancing
    oggpack_buffer z = { 1, 0, one, one, 1 };
    g_advTotal = 0;
    if (decode_packed_entry_number(&book, &z) != -1 || g_advTotal != 0)
    { std::printf("00c96320_TEST FAIL\n"); return 1; }
    if (bitreverse(1u) != 0x80000000u || bitreverse(0x0000000Fu) != 0xF0000000u)
    { std::printf("00c96320_TEST FAIL\n"); return 1; }
    std::printf("00c96320_TEST PASS\n");
    return 0;
}