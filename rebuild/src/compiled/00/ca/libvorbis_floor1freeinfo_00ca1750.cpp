// libvorbis::floor1_free_info(vorbis_info_floor* i) -- libvorbis floor1.c: wipe a floor-1 setup (vorbis_info_floor1, 0x460 bytes) and free it
// Leaf: @floor1_free_info@4
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1750, 30 bytes (+ int3 padding). libvorbis lib/floor1.c (libpk's statically linked Xiph
//   libvorbis 20030909); i in ecx, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 448 "0xCA1750 = floor1_free_info"): its address is the free_info entry of the
//   floor1 function table at 0x013AC234 (floor1_exportbundle), and floor1_unpack's error path calls it (0x00CA1BA2); 0x118 dwords
//   is sizeof(vorbis_info_floor1) in 1.0.1 (partitions, partitionclass[31], class_dim/subs/book[16], class_subbook[16][8], mult,
//   postlist[65], five encoder floats, n = 280 words).
// Callee: free (MSVCR71 import [0x01440154]) through _ogg_free.
// Shape: the shipped source; memset(info,0,sizeof(*info)) is the intrinsic `rep stosd`.
// Label: the landed ClearAndFreeBlock is a descriptive placeholder; the real name is the floor1.c static `floor1_free_info`.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef void vorbis_info_floor;

#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef struct {
    int partitions;                     /* 0 to 31 */
    int partitionclass[VIF_PARTS];      /* 0 to 15 */
    int class_dim[VIF_CLASS];           /* 1 to 8 */
    int class_subs[VIF_CLASS];          /* 0,1,2,3 (bits: 1<<n poss) */
    int class_book[VIF_CLASS];          /* subs ^ dim entries */
    int class_subbook[VIF_CLASS][8];    /* [VIF_CLASS][subs] */
    int mult;                           /* 1 2 3 or 4 */
    int postlist[VIF_POSIT + 2];        /* first two implicit */
    /* encode side analysis parameters */
    float maxover;
    float maxunder;
    float maxerr;
    float twofitweight;
    float twofitatten;
    int n;
} vorbis_info_floor1;                   // 0x460 bytes

extern "C" void __fastcall floor1_free_info(vorbis_info_floor* i)
{
    vorbis_info_floor1* info = (vorbis_info_floor1*)i;
    if (info) {
        memset(info, 0, sizeof(*info));
        _ogg_free(info);
    }
}
