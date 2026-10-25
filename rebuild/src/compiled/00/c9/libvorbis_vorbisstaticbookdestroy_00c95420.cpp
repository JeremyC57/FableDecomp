// libvorbis::vorbis_staticbook_destroy(static_codebook* b) -- clear and free a static codebook that was heap-allocated (allocedp); others are left alone
// Leaf: @vorbis_staticbook_destroy@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95420, 27 bytes, b in ecx, plain `ret`. libvorbis 1.0.1 lib/sharedbook.c.
//   Callees: vorbis_staticbook_clear 0x00C95370 (landed, agent447) and free (MSVCR71 import [0x01440154]) through
//   _ogg_free.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the neighbouring landed
//   agent447 rows (vorbis_staticbook_clear 0x00C95370, vorbis_book_decodev_set 0x00C96700, vorbis_book_errorv 0x00C96170)
//   use the same libvorbis 1.0.1 structs.
// Label: catalog "CEngineResourceManager::WorldFrameUpdate" is propagated. Its source modelled the free import as a
//   DEFINED data global `void (__cdecl *g_afterUpdate)(...) = 0` (agent447 LESSONS: "the model is wrong"); here it is the
//   real dllimport.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
#define _ogg_free free

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

extern "C" void __fastcall vorbis_staticbook_clear(static_codebook* b);   // 0x00C95370

extern "C" void __fastcall vorbis_staticbook_destroy(static_codebook* b)
{
    if (b->allocedp) {
        vorbis_staticbook_clear(b);
        _ogg_free(b);
    }
}
