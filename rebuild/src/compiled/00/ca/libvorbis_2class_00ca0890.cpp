// libvorbis::_2class(vorbis_block* vb, vorbis_look_residue* vl, float** in, int ch) -- libvorbis res0.c: residue-2 classification of an interleaved channel vector, one class word per partition from the magnitude / angle maxima against the info's class metrics
// Leaf: __2class@16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA0890, 776 bytes. libvorbis lib/res0.c (Xiph "libVorbis I 20030909", post-1.0.1 CVS, GOTCHAS
//   0xC89xxx..0xC9Exxx / 0xC908E0..0xC9FDF0); a C static compiled with VC7.1's private convention (vb in EAX, vl/in/ch on the stack,
//   callee-clean `ret 0Ch`), which `extern "C" { static __declspec(noinline) __stdcall }` with its real caller in the TU reproduces
//   (GOTCHAS chunks 585/586/599).
// Identity (proven): (1) its only caller is the landed res2_class 0x00CA1310 (`if (used) return _2class(vb, vl, in, ch)`), slot 5 of
//   residue2_exportbundle 0x013ABE18, which binds this address as `__2class@16`; (2) the body is res0.c's `_2class` verbatim: two
//   _vorbis_block_alloc 0x00C8F0D0 calls (partword, partword[0] of n*ch/samples_per_partition longs, memset), the partition loop with
//   fabs maxima over in[0][l] (magnitude) and in[1..ch)[l] (angle), the classmetric1/classmetric2 break and `look->frames++`.
//   The catalog label "CurveInterpolation_ResampleCurveData" is a descriptive stand-in.
// Callees: _vorbis_block_alloc 0x00C8F0D0 (`__fastcall`, landed), memset (inline rep stos).
// Shape: the landed res2_class TU (0x00CA1310) holds this body unchanged; graded here on its own leaf. VC7.1 unrolls the inner
//   k loop by four itself; `fabs` is C's double fabs (intrinsic), as in the landed _01class 0x00CA0680: the maxima are then
//   compared as doubles (`fld [max]; fld st(1); fcompp`) and live in frame slots (angmax in the dead `ch` argument slot).
typedef struct vorbis_block vorbis_block;
typedef void vorbis_look_residue;

extern "C" void* __cdecl memset(void*, int, unsigned int);
extern "C" double __cdecl fabs(double);     // C fabs: a double compare against the float maxima (the <math.h> float overload gives a direct fcom)
#pragma intrinsic(fabs, memset)
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
extern "C" void* __fastcall _vorbis_block_alloc(vorbis_block* vb, long bytes);   // block.c 0x00C8F0D0

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
                                        float** in, int* nonzero, int ch)   // 0x00CA1310 (landed), the caller that gives _2class its convention
{
    int i, used = 0;
    for (i = 0; i < ch; i++)
        if (nonzero[i]) used++;
    if (used)
        return (_2class(vb, vl, in, ch));
    else
        return (0);
}