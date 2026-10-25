// libvorbis::vorbis_book_init_encode(codebook* c, const static_codebook* s) -- set up an encode codebook from a static one: zero it, copy entries/dim, build the codeword list and the unquantized value list; returns 0
// Leaf: @vorbis_book_init_encode@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C954A0, 74 bytes (+ int3 padding), c in ecx, s in edx, plain `ret`.
//   libvorbis 1.0.1 lib/sharedbook.c. Callees: _make_words(long* l, long n, long sparsecount) @ 0x00C94FD0 and
//   _book_unquantize(const static_codebook* b, int n, int* sparsemap) @ 0x00C951B0 (both `__fastcall`, third argument 0
//   on the stack; manifest "Bitstream_ProcessCodes" / "Waveform_Synthesize").
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the neighbouring landed
//   agent447 rows (vorbis_staticbook_clear 0x00C95370, vorbis_book_decodev_set 0x00C96700, vorbis_book_errorv 0x00C96170)
//   use the same libvorbis 1.0.1 structs.
// Label: catalog "InitCodecStage_00c954a0" / manifest "CEntry::CEntry" are descriptive/propagated.
// Shape: the shipped source; `rep stosd` is the 0x2c-byte memset, then c->c, entries, used_entries, dim in source order,
//   codelist (+0x14) before valuelist (+0x10).
#include <string.h>
typedef struct static_codebook {                   // libvorbis 1.0.1 codebook.h, 0x34 bytes
    long  dim;                                     // +0x00
    long  entries;                                 // +0x04
    long* lengthlist;                              // +0x08
    int   maptype;                                 // +0x0c
    long  q_min;                                   // +0x10
    long  q_delta;                                 // +0x14
    int   q_quant;                                 // +0x18
    int   q_sequencep;                             // +0x1c
    long* quantlist;                               // +0x20
    struct encode_aux_nearestmatch* nearest_tree;  // +0x24
    struct encode_aux_threshmatch*  thresh_tree;   // +0x28
    struct encode_aux_pigeonhole*   pigeon_tree;   // +0x2c
    int   allocedp;                                // +0x30
} static_codebook;
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

extern "C" ogg_uint32_t* __fastcall _make_words(long* l, long n, long sparsecount);               // 0x00C94FD0
extern "C" float* __fastcall _book_unquantize(const static_codebook* b, int n, int* sparsemap);    // 0x00C951B0

extern "C" int __fastcall vorbis_book_init_encode(codebook* c, const static_codebook* s)
{
    memset(c, 0, sizeof(*c));
    c->c = s;
    c->entries = s->entries;
    c->used_entries = s->entries;
    c->dim = s->dim;
    c->codelist = _make_words(s->lengthlist, s->entries, 0);
    c->valuelist = _book_unquantize(s, s->entries, 0);

    return (0);
}
