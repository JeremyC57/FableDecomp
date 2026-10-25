// libvorbis::vorbis_book_decodev_set(codebook* book, float* a, oggpack_buffer* b, int n) -- decode codebook entries until n values are filled, copying each entry's vector into a; -1 on end of packet
// Leaf: vorbis_book_decodev_set
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C96700, 92 bytes. libvorbis 1.0.1 lib/codebook.c, compiled as C with
//   /Gr: `__fastcall`, book in ecx, a in edx, b and n on the stack (`ret 8`).
// Callees: decode_packed_entry_number 0x00C96320 -- a `STIN` (static inline) helper that LTCG left out of line
//   with both arguments on the stack and a callee pop (`ret 8`); declared `__stdcall`, which is that convention.
// Layout: libvorbis 1.0.1 codebook (+0x00 dim, +0x10 valuelist).
// Shape: the shipped source; the inner copy re-reads book->dim per element; the -1 exit is its own block after
//   the success return.
typedef unsigned int ogg_uint32_t;

typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

typedef struct codebook {
    long          dim;
    long          entries;
    long          used_entries;
    const void*   c;
    float*        valuelist;
    ogg_uint32_t* codelist;
    int*          dec_index;
    char*         dec_codelengths;
    ogg_uint32_t* dec_firsttable;
    int           dec_firsttablen;
    int           dec_maxlength;
} codebook;

extern "C" long __stdcall decode_packed_entry_number(codebook* book, oggpack_buffer* b);   // 0x00C96320

extern "C" long __fastcall vorbis_book_decodev_set(codebook* book, float* a, oggpack_buffer* b, int n)
{
    int i, j, entry;
    float* t;

    for (i = 0; i < n;) {
        entry = decode_packed_entry_number(book, b);
        if (entry == -1) return -1;
        t = book->valuelist + entry * book->dim;
        for (j = 0; j < book->dim;)
            a[i++] = t[j++];
    }
    return 0;
}