// libvorbis::floor0_free_look(vorbis_look_floor* i) -- libvorbis floor0.c: free a floor-0 look (both linear bark maps, the map array), wipe it and free it
// Leaf: @floor0_free_look@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA35A0, 94 bytes (+2 int3). libvorbis lib/floor0.c (libpk's statically linked Xiph
//   libvorbis, "Xiph.Org libVorbis I 20030909"): the free_look slot of the floor0 function table (its address sits at
//   0x013AC254 in .data, next to floor0_free_info 0x00CA3580); `__fastcall`, i in ecx, plain `ret`.
// Callee: free (MSVCR71 import [0x01440154], hoisted into edi) through _ogg_free.
// Layout: libvorbis floor0.c vorbis_look_floor0 {ln, m, linearmap, n[2], vi, bits, frames} (0x20 bytes).
// Shape: the shipped source; memset(look,0,0x20) is the intrinsic (`xor edx,edx` + eight stores through eax), the last
//   free's `push esi` scheduled between the stores.
// Label: manifest "C3DGeneratorObject_Release" is propagated; the real name is the floor0.c static `floor0_free_look`.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef void vorbis_look_floor;
typedef struct vorbis_info_floor0 vorbis_info_floor0;

typedef struct {
    int ln;                     // +0x00
    int m;                      // +0x04
    int** linearmap;            // +0x08
    int n[2];                   // +0x0c
    vorbis_info_floor0* vi;     // +0x14
    long bits;                  // +0x18
    long frames;                // +0x1c
} vorbis_look_floor0;

extern "C" void __fastcall floor0_free_look(vorbis_look_floor* i)
{
    vorbis_look_floor0* look = (vorbis_look_floor0*)i;
    if (look) {

        if (look->linearmap) {

            if (look->linearmap[0]) _ogg_free(look->linearmap[0]);
            if (look->linearmap[1]) _ogg_free(look->linearmap[1]);

            _ogg_free(look->linearmap);
        }
        memset(look, 0, sizeof(*look));
        _ogg_free(look);
    }
}