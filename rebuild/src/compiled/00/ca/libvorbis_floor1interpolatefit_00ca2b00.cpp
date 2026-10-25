// libvorbis::floor1_interpolate_fit(vorbis_block* vb, vorbis_look_floor1* look, int* A, int* B, int del) -- libvorbis floor1.c: blend two fitted floor-1 post sets 16.16 by del into a block-arena array, keeping the "unused" bit 0x8000 only where both inputs carry it
// Leaf: @floor1_interpolate_fit@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA2B00, 166 bytes (+10 int3). libvorbis lib/floor1.c (libpk's statically linked Xiph
//   libvorbis, "Xiph.Org libVorbis I 20030909"); `__fastcall` (vb in ecx, look in edx, A/B/del on the stack, `ret 0Ch`),
//   called from the encoder's mapping0_forward 0x00C9F3D5 / 0x00C9F438.
// Callee: _vorbis_block_alloc 0x00C8F0D0 (`__fastcall`, vb in ecx, bytes in edx; landed by agent446).
// Layout: vorbis_look_floor1 (1.0.1 floor1.c, VIF_POSIT 63): sorted/forward/reverse_index[65], hineighbor/loneighbor[63],
//   posts at +0x504.
// Shape: the shipped source; VC7.1 walks B by pointer (ecx), addresses A and output as offsets from it, and counts
//   posts down in ebp.
// Label: manifest "PixelBlend_InterpolateWithMemAlloc" is a descriptive placeholder; the real name is the libvorbis API.
#define VIF_POSIT 63

typedef struct vorbis_block vorbis_block;
typedef struct vorbis_info_floor1 vorbis_info_floor1;

typedef struct {
    int sorted_index[VIF_POSIT + 2];   // +0x000
    int forward_index[VIF_POSIT + 2];  // +0x104
    int reverse_index[VIF_POSIT + 2];  // +0x208
    int hineighbor[VIF_POSIT];         // +0x30c
    int loneighbor[VIF_POSIT];         // +0x408
    int posts;                         // +0x504
    int n;                             // +0x508
    int quant_q;                       // +0x50c
    vorbis_info_floor1* vi;            // +0x510
    long phrasebits;                   // +0x514
    long postbits;                     // +0x518
    long frames;                       // +0x51c
} vorbis_look_floor1;

extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block* vb, long bytes);   // 0x00C8F0D0

extern "C" int* __fastcall floor1_interpolate_fit(vorbis_block* vb, vorbis_look_floor1* look,
                                                  int* A, int* B,
                                                  int del)
{
    long i;
    long posts = look->posts;
    int* output = 0;

    if (A && B) {
        output = (int*)_vorbis_block_alloc(vb, sizeof(*output) * posts);

        for (i = 0; i < posts; i++) {
            output[i] = ((65536 - del) * (A[i] & 0x7fff) + del * (B[i] & 0x7fff) + 32768) >> 16;
            if (A[i] & 0x8000 && B[i] & 0x8000) output[i] |= 0x8000;
        }
    }

    return (output);
}