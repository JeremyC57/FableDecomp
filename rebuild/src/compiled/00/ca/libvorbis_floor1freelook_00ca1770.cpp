// libvorbis::floor1_free_look(vorbis_look_floor* i) -- Xiph floor1.c: wipe and free a floor 1 look (0x520 bytes)
// Leaf: @floor1_free_look@4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA1770, 30 bytes, __fastcall (i in ecx), plain ret.
// Callees: free (MSVCR71 import [0x01440154]); memset is the rep stosd intrinsic (0x148 dwords).
// Library: libpk statically links the Xiph Ogg Vorbis decoder ("Xiph.Org libVorbis I 20030909" @0x012BF240; libogg 1.1 /
//   libvorbis post-1.0.1 CVS), compiled as C with /Gr: the functions are extern "C" __fastcall.
// Identity: the free_look slot (+0x10) of floor1_exportbundle at 0x013AC228 (pack 0x00CA17C0, unpack 0x00CA1970, look
//   0x00CA1BE0, free_info 0x00CA1750, free_look = this, inverse1 0x00CA3150, inverse2 0x00CA3420); 0x148 dwords =
//   sizeof(vorbis_look_floor1): sorted/forward/reverse_index[65], hi/loneighbor[63], posts, n, quant_q, vi, phrasebits,
//   postbits, frames. The catalog label (gap-scan) is a placeholder.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free
#define VIF_POSIT 63

typedef void vorbis_look_floor;
typedef struct vorbis_info_floor1 vorbis_info_floor1;

typedef struct {
    int sorted_index[VIF_POSIT + 2];
    int forward_index[VIF_POSIT + 2];
    int reverse_index[VIF_POSIT + 2];

    int hineighbor[VIF_POSIT];
    int loneighbor[VIF_POSIT];
    int posts;

    int n;
    int quant_q;
    vorbis_info_floor1* vi;

    long phrasebits;
    long postbits;
    long frames;
} vorbis_look_floor1;

extern "C" void __fastcall floor1_free_look(vorbis_look_floor* i)
{
    vorbis_look_floor1* look = (vorbis_look_floor1*)i;
    if (look) {
        memset(look, 0, sizeof(*look));
        _ogg_free(look);
    }
}