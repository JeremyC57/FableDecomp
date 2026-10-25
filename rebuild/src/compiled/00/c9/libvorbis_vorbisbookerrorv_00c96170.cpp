// libvorbis::vorbis_book_errorv(codebook* book, float* a) -- replace a vector with its nearest codebook entry (single-stage search) and return the entry number
// Leaf: vorbis_book_errorv
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C96170, 158 bytes. libvorbis 1.0.1 lib/codebook.c, compiled as C with
//   /Gr: `__fastcall`, book in ecx, a in edx, plain `ret`.
// Callees: _best 0x00C95A10 (sharedbook.c, `__fastcall` book/a in ecx/edx, step 1 on the stack).
// Layout: libvorbis 1.0.1 codebook (+0x00 dim, +0x10 valuelist).
// Shape: the shipped source; the copy loop is unrolled by four and re-reads book->valuelist for every element
//   (the float stores may alias it), with the remainder loop after; best is kept in a frame slot.
typedef unsigned int ogg_uint32_t;

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

extern "C" int __fastcall _best(codebook* book, float* a, int step);   // 0x00C95A10

extern "C" int __fastcall vorbis_book_errorv(codebook* book, float* a)
{
    int dim = book->dim, k;
    int best = _best(book, a, 1);
    for (k = 0; k < dim; k++)
        a[k] = (book->valuelist + best * dim)[k];
    return best;
}