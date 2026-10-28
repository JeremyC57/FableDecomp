// libvorbis::_encodepart(oggpack_buffer* opb, float* vec, int n, codebook* book, long* acc) -- residue encoder: code n/dim vectors of vec with their best codebook entries and return the number of bits written
// Leaf: @_encodepart@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA0620, 81 bytes, opb in ecx, vec in edx, n/book/acc on the stack (`ret 0Ch`).
//   libvorbis lib/res0.c (Xiph.Org libVorbis I 20030909 @ 0x00EBF240, compiled as C with /Gr: `__fastcall`).
// Identity: the res0.c body `step = n/dim; for (i < step) { entry = local_book_besterror(book, vec+i*dim);
//   bits += vorbis_book_encode(book, entry, opb); } return bits;` (TRAIN_RES off, so acc is unused). It sits directly
//   before _01class 0x00CA0680 (landed binding `__01class@16`), as in res0.c; its callers are the two residue
//   encoders' function-pointer arguments (`abs .text` refs at 0x00CA1178 / 0x00CA146C, i.e. _01forward's encode
//   argument). Label correction: the manifest row "CEnvironment::LookupLocalThemeIndex" is propagated.
// Callees: local_book_besterror 0x00CA02B0 (static, two stack arguments, `ret 8`: declared `__stdcall`, the landed
//   _01class convention), vorbis_book_encode 0x00C96140 (landed, `@vorbis_book_encode@12`).
// Shape: opb is spilled to the frame across the calls; `dim*4` is homed in the dead `n` slot; the counter runs down in ebp.
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;
struct static_codebook;
typedef unsigned int ogg_uint32_t;
typedef struct codebook {                          // libvorbis codebook.h, 0x2c bytes
    long                          dim;             // +0x00
    long                          entries;         // +0x04
    long                          used_entries;    // +0x08
    const struct static_codebook* c;               // +0x0c
    float*                        valuelist;       // +0x10
    ogg_uint32_t*                 codelist;        // +0x14
    int*                          dec_index;       // +0x18
    char*                         dec_codelengths; // +0x1c
    ogg_uint32_t*                 dec_firsttable;  // +0x20
    int                           dec_firsttablen; // +0x24
    int                           dec_maxlength;   // +0x28
} codebook;

extern "C" int __stdcall local_book_besterror(codebook* book, float* a);              // 0x00CA02B0
extern "C" int __fastcall vorbis_book_encode(codebook* book, int a, oggpack_buffer* b);  // 0x00C96140

extern "C" int __fastcall _encodepart(oggpack_buffer* opb, float* vec, int n, codebook* book, long* acc)
{
    int i, bits = 0;
    int dim = book->dim;
    int step = n / dim;

    for (i = 0; i < step; i++) {
        int entry = local_book_besterror(book, vec + i * dim);
        bits += vorbis_book_encode(book, entry, opb);
    }

    return (bits);
}