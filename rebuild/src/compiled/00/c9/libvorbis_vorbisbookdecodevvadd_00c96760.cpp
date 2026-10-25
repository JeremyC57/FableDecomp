// libvorbis::vorbis_book_decodevv_add(codebook* book, float** a, long offset, int ch, oggpack_buffer* b, int n) -- decode codebook vectors and add them channel-interleaved into the ch output vectors from offset/ch on; -1 on end of packet
// Leaf: vorbis_book_decodevv_add
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C96760, 149 bytes. libvorbis 1.0.1 lib/codebook.c, compiled as C with
//   /Gr: `__fastcall`, book in ecx, a in edx (homed in the `push ecx` slot), offset/ch/b/n on the stack
//   (`ret 10h`).
// Callees: decode_packed_entry_number 0x00C96320 (static inline helper LTCG kept out of line with stack
//   arguments and a callee pop -- declared `__stdcall`).
// Layout: libvorbis 1.0.1 codebook (+0x00 dim, +0x10 valuelist).
// Shape: the shipped source; `(offset + n) / ch` is computed once into a frame slot; `a[chptr++][i] += t[j]`
//   is `fld t[j] ; fadd a[chptr][i] ; fstp`.
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

extern "C" long __fastcall vorbis_book_decodevv_add(codebook* book, float** a, long offset, int ch,
                                                    oggpack_buffer* b, int n)
{
    long i, j, entry;
    int chptr = 0;

    for (i = offset / ch; i < (offset + n) / ch;) {
        entry = decode_packed_entry_number(book, b);
        if (entry == -1) return -1;
        {
            const float* t = book->valuelist + entry * book->dim;
            for (j = 0; j < book->dim; j++) {
                a[chptr++][i] += t[j];
                if (chptr == ch) {
                    chptr = 0;
                    i++;
                }
            }
        }
    }
    return 0;
}