// libvorbis::vorbis_book_clear(codebook* b) -- Xiph sharedbook.c: free a decode/encode codebook's value list, code list and decode tables (the static book belongs to the info), then wipe the 0x2C-byte record
// Leaf: @vorbis_book_clear@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C95440, 87 bytes, __fastcall (b in ecx), plain ret.
// Callees: free (MSVCR71 import [0x01440154], hoisted into esi) through _ogg_free; memset is the rep stosd intrinsic.
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: five null-tested frees of +0x10..+0x20 then memset(b,0,0x2C) = sharedbook.c's vorbis_book_clear over the
//   codebook record (dim, entries, used_entries, c, valuelist +0x10, codelist +0x14, dec_index +0x18, dec_codelengths
//   +0x1c, dec_firsttable +0x20, dec_firsttablen, dec_maxlength); it follows the landed vorbis_staticbook_clear 0x00C95370
//   and vorbis_staticbook_destroy 0x00C95420 and precedes vorbis_book_init_encode 0x00C954A0, sharedbook.c's order.
//   Callers 0x00C8E0A8 and 0x00C955F2. Label correction: the catalog's "CWound::Kill" is propagated.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free
typedef unsigned int ogg_uint32_t;

typedef struct static_codebook static_codebook;

typedef struct codebook {
    long dim;                           // +0x00 codebook dimensions (elements per vector)
    long entries;                       // +0x04 codebook entries
    long used_entries;                  // +0x08 populated codebook entries
    const static_codebook* c;           // +0x0c
    float* valuelist;                   // +0x10 list of dim*entries actual entry values
    ogg_uint32_t* codelist;             // +0x14 list of bitstream codewords for each entry
    int* dec_index;                     // +0x18 only used if sparseness collapsed
    char* dec_codelengths;              // +0x1c
    ogg_uint32_t* dec_firsttable;       // +0x20
    int dec_firsttablen;                // +0x24
    int dec_maxlength;                  // +0x28
} codebook;

extern "C" void __fastcall vorbis_book_clear(codebook* b)
{
    /* static book is not cleared; we're likely called on the lookup and
       the static codebook belongs to the info struct */
    if (b->valuelist) _ogg_free(b->valuelist);
    if (b->codelist) _ogg_free(b->codelist);

    if (b->dec_index) _ogg_free(b->dec_index);
    if (b->dec_codelengths) _ogg_free(b->dec_codelengths);
    if (b->dec_firsttable) _ogg_free(b->dec_firsttable);

    memset(b, 0, sizeof(*b));
}