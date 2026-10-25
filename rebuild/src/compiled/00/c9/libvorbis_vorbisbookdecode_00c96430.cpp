// libvorbis::vorbis_book_decode(codebook* book, oggpack_buffer* b) -- decode the next packed entry and map it through dec_index; the negative status when decoding fails
// Leaf: @vorbis_book_decode@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C96430, 22 bytes, book in ecx, b in edx, plain `ret`. libvorbis 1.0.1
//   lib/codebook.c. Callee: decode_packed_entry_number 0x00C96320 -- a codebook.c `STIN` helper LTCG left out of line,
//   `push b ; push book ; call` with a callee pop, so plain `__stdcall` at the call site (agent447 LESSONS; the landed
//   vorbis_book_decodev_set 0x00C96700 binds the same `_decode_packed_entry_number@8`).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the neighbouring landed
//   agent447 rows (vorbis_staticbook_clear 0x00C95370, vorbis_book_decodev_set 0x00C96700, vorbis_book_errorv 0x00C96170)
//   use the same libvorbis 1.0.1 structs.
// Label: catalog "CDefinitionManager::GetPDefFromClassIndex" is propagated; its "ValidateClassIndex" was
//   decode_packed_entry_number and its m_classArray (+0x18) is codebook::dec_index.
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

extern "C" long __stdcall decode_packed_entry_number(codebook* book, oggpack_buffer* b);   // 0x00C96320

extern "C" long __fastcall vorbis_book_decode(codebook* book, oggpack_buffer* b)
{
    long packed_entry = decode_packed_entry_number(book, b);
    if (packed_entry >= 0)
        return (book->dec_index[packed_entry]);

    /* if there's no dec_index, the codebook unpacking isn't collapsed */
    return (packed_entry);
}
