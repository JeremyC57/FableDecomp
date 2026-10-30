// libvorbis::res2_class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int* nonzero, int ch) -- libvorbis res0.c: residue-2 classification; counts the nonzero channels and, if any, classifies the interleaved vector through _2class
// Shape (retry1): parked as an "LTCG custom register convention" gap (callee); with res0.c's static _2class body in the TU as `extern "C" { static __declspec(noinline) __stdcall }`, VC7.1 /O2 gives it retail's private convention itself (vb in EAX, vl/in/ch pushed, callee-pop), so the call site matches.
// Leaf: @res2_class@20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1310, 66 bytes (+14 int3). libvorbis lib/res0.c; slot 5 (class) of residue2_exportbundle
//   at 0x013ABE18 (res0_pack 0x00C9FEA0, res0_unpack 0x00C9FF80, res0_look 0x00CA00E0, res0_free_info 0x00C9FDD0,
//   res0_free_look 0x00C9FDF0, res2_class, res2_forward 0x00CA1360, res2_inverse 0x00CA15C0); `__fastcall`, `ret 0Ch`.
// Callee: _2class 0x00CA0890 (res0.c static: vb in eax, vl/in/ch on the stack, callee-clean); its libvorbis 1.0.1 body sits in the TU (not graded here).
// Label: manifest "Geometry_ProcessNonzeroElements" is a descriptive placeholder; the real name is the libvorbis res2_class.
typedef struct vorbis_block vorbis_block;
typedef void vorbis_look_residue;

#include <string.h>
#include <math.h>
typedef struct vorbis_info_residue0 {    // libvorbis 1.0.1 backends.h
    long begin;
    long end;
    int grouping;
    int partitions;
    int groupbook;
    int secondstages[64];
    int booklist[256];
    float classmetric1[64];
    float classmetric2[64];
} vorbis_info_residue0;
typedef struct {                         // libvorbis 1.0.1 res0.c
    vorbis_info_residue0* info;
    int parts;
    int stages;
    void* fullbooks;
    void* phrasebook;
    void*** partbooks;
    int partvals;
    int** decodemap;
    long postbits;
    long phrasebits;
    long frames;
} vorbis_look_residue0;
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block* vb, long bytes);   // block.c

extern "C" {
static __declspec(noinline) long** __stdcall _2class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int ch)   // 0x00CA0890
{
    long i, j, k, l;
    vorbis_look_residue0* look = (vorbis_look_residue0*)vl;
    vorbis_info_residue0* info = look->info;

    /* move all this setup out later */
    int samples_per_partition = info->grouping;
    int possible_partitions = info->partitions;
    int n = info->end - info->begin;

    int partvals = n / samples_per_partition;
    long** partword = (long**)_vorbis_block_alloc(vb, sizeof(*partword));
    float magmax = 0.f;
    float angmax = 0.f;

    partword[0] = (long*)_vorbis_block_alloc(vb, n * ch / samples_per_partition * sizeof(*partword[0]));
    memset(partword[0], 0, n * ch / samples_per_partition * sizeof(*partword[0]));

    for (i = 0, l = info->begin / ch; i < partvals; i++) {
        magmax = 0.f;
        angmax = 0.f;
        for (j = 0; j < samples_per_partition; j += ch) {
            if (fabs(in[0][l]) > magmax) magmax = fabs(in[0][l]);
            for (k = 1; k < ch; k++)
                if (fabs(in[k][l]) > angmax) angmax = fabs(in[k][l]);
            l++;
        }

        for (j = 0; j < possible_partitions - 1; j++)
            if (magmax <= info->classmetric1[j] &&
                angmax <= info->classmetric2[j])
                break;

        partword[0][i] = j;
    }

    look->frames++;
    return (partword);
}
}

extern "C" long** __fastcall res2_class(vorbis_block* vb, vorbis_look_residue* vl,
                                        float** in, int* nonzero, int ch)
{
    int i, used = 0;
    for (i = 0; i < ch; i++)
        if (nonzero[i]) used++;
    if (used)
        return (_2class(vb, vl, in, ch));
    else
        return (0);
}