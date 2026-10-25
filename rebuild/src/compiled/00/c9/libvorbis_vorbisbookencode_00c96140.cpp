// libvorbis::vorbis_book_encode(codebook* book, int a, oggpack_buffer* b) -- write entry a's codeword with its length in bits and return that length
// Leaf: @vorbis_book_encode@12
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C96140, 43 bytes (+ int3 padding), book in ecx, a in edx, b on the stack
//   (`ret 4`). libvorbis 1.0.1 lib/codebook.c. Callee: oggpack_write 0x00C90FC0 (`__fastcall`).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the neighbouring landed
//   agent447 rows (vorbis_staticbook_clear 0x00C95370, vorbis_book_decodev_set 0x00C96700, vorbis_book_errorv 0x00C96170)
//   use the same libvorbis 1.0.1 structs.
// Label: catalog "CCodeTable_00c96140::WriteCode" (comment said libpk) / manifest
//   "CEnvironment::GetLocalEnvironmentThemeIndexFromGlobal" are descriptive/propagated.
// Shape: the shipped source; the length is reached through book->c (+0x0c) -> lengthlist (+0x08) and re-read after the
//   call for the return value; the codeword is codelist (+0x14)[a].
typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;
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

extern "C" void __fastcall oggpack_write(oggpack_buffer* b, unsigned long value, int bits);   // 0x00C90FC0

extern "C" int __fastcall vorbis_book_encode(codebook* book, int a, oggpack_buffer* b)
{
    oggpack_write(b, book->codelist[a], book->c->lengthlist[a]);
    return (book->c->lengthlist[a]);
}
