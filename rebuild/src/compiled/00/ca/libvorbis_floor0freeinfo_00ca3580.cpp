// libvorbis::floor0_free_info(vorbis_info_floor* i) -- libvorbis floor0.c: wipe a floor-0 setup (vorbis_info_floor0, 96 bytes) and free it
// Leaf: @floor0_free_info@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3580, 30 bytes (+ int3 padding). libvorbis lib/floor0.c (libpk's statically linked Xiph
//   libvorbis 20030909); i in ecx, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 448 "0xCA3580 = floor0_free_info (source calls CRT free as g_ctorptr)"): its
//   address is the free_info entry of the floor0 function table (0x013AC250, beside agent448's landed floor0_free_look 0x00CA35A0
//   at 0x013AC254), and floor0_unpack's error path calls it (0x00CA36D0); 0x18 dwords is sizeof(vorbis_info_floor0) (order, rate,
//   barkmap, ampbits, ampdB, numbooks, books[16], lessthan, greaterthan).
// Fix of the call: `call [0x01440154]` is the MSVCR71 `free` import (as in floor0_free_look), not a constructor pointer.
// Shape: the shipped source; memset(info,0,sizeof(*info)) is the intrinsic `rep stosd`.
// Label: the landed `_Cons_val<allocator<C3DGeneratorObject>,...>` is a propagated label; the real name is the floor0.c static.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef void vorbis_info_floor;

typedef struct {
    int order;
    long rate;
    long barkmap;
    int ampbits;
    int ampdB;
    int numbooks;                       /* <= 16 */
    int books[16];
    float lessthan;                     /* encode-only config setting hacks for libvorbis */
    float greaterthan;                  /* encode-only config setting hacks for libvorbis */
} vorbis_info_floor0;                   // 96 bytes

extern "C" void __fastcall floor0_free_info(vorbis_info_floor* i)
{
    vorbis_info_floor0* info = (vorbis_info_floor0*)i;
    if (info) {
        memset(info, 0, sizeof(*info));
        _ogg_free(info);
    }
}
