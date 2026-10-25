// libvorbis::res0_free_look(vorbis_look_residue* i) -- free a residue 0/1/2 look: the per-partition book arrays, the decode maps, then the look itself (wiped first)
// Leaf: res0_free_look
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9FDF0, 113 bytes. libvorbis 1.0.1 lib/res0.c (the free_look slot of the
//   residue0/1/2 function tables), compiled as C with /Gr: `__fastcall`, i in ecx, plain `ret`.
// Callees: free (MSVCR71 import [0x01440154], hoisted into ebx) through _ogg_free.
// Layout: libvorbis 1.0.1 vorbis_look_residue0 without TRAIN_RES (0x2c bytes: info, parts, stages, fullbooks,
//   phrasebook, partbooks +0x14, partvals +0x18, decodemap +0x1c, postbits, phrasebits, frames).
// Shape: the shipped source; partbooks[j] is null-tested, decodemap[j] is not; the final free's `push esi`
//   is scheduled before the `rep stosd` of memset(look,0,0x2c) and shares one `add esp,8`.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef void vorbis_look_residue;
typedef struct codebook codebook;

typedef struct vorbis_look_residue0 {
    void*       info;
    int         parts;
    int         stages;
    codebook*   fullbooks;
    codebook*   phrasebook;
    codebook*** partbooks;
    int         partvals;
    int**       decodemap;
    long        postbits;
    long        phrasebits;
    long        frames;
} vorbis_look_residue0;

extern "C" void __fastcall res0_free_look(vorbis_look_residue* i)
{
    int j;
    if (i) {
        vorbis_look_residue0* look = (vorbis_look_residue0*)i;

        for (j = 0; j < look->parts; j++)
            if (look->partbooks[j]) _ogg_free(look->partbooks[j]);
        _ogg_free(look->partbooks);
        for (j = 0; j < look->partvals; j++)
            _ogg_free(look->decodemap[j]);
        _ogg_free(look->decodemap);

        memset(look, 0, sizeof(*look));
        _ogg_free(look);
    }
}