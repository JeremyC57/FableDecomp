// libvorbis::_vp_noise_normalize_sort(vorbis_look_psy* p, float* magnitudes, int* sortedindex) -- per partition, sort the magnitude indices by descending |value| (from normal_start on) for noise normalisation
// Leaf: _vp_noise_normalize_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C94750, 185 bytes. libvorbis 1.0.1 lib/psy.c, compiled as C with /Gr:
//   `__fastcall`, p in ecx, magnitudes in edx, sortedindex on the stack (`ret 4`); an ebp frame for the alloca.
// Callees: _alloca's probe __chkstk 0x00BFEA30 (the partition-sized work array), qsort (MSVCR71 import
//   [0x01440060]) with the comparator apsort 0x00C945B0 pushed as an immediate.
// Layout: libvorbis 1.0.1 vorbis_look_psy (+0 n, +4 vi) and vorbis_info_psy (+0x1fc normal_start, +0x200
//   normal_partition; max_curve_dB at +0x1f0, normal_thresh, an 8-aligned double, at +0x208).
// Shape: the shipped source; n, magnitudes and start live in frame slots, `work` is esp after the probe
//   (`mov edi,esp`), and `work[i] - magnitudes` is a pointer difference (`sar edx,2`).
extern "C" void* __cdecl _alloca(unsigned int size);
#pragma intrinsic(_alloca)
extern "C" __declspec(dllimport) void __cdecl qsort(void* base, unsigned int num, unsigned int width,
                                                     int (__cdecl* compare)(const void*, const void*));   // [0x01440060]
extern "C" int __cdecl apsort(const void* a, const void* b);   // 0x00C945B0

#define P_BANDS 17
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40

typedef struct vorbis_info_psy {
    int    blockflag;
    float  ath_adjatt;
    float  ath_maxatt;
    float  tone_masteratt[P_NOISECURVES];
    float  tone_centerboost;
    float  tone_decay;
    float  tone_abs_limit;
    float  toneatt[P_BANDS];
    int    noisemaskp;
    float  noisemaxsupp;
    float  noisewindowlo;
    float  noisewindowhi;
    int    noisewindowlomin;
    int    noisewindowhimin;
    int    noisewindowfixed;
    float  noiseoff[P_NOISECURVES][P_BANDS];
    float  noisecompand[NOISE_COMPAND_LEVELS];
    float  max_curve_dB;
    int    normal_channel_p;
    int    normal_point_p;
    int    normal_start;
    int    normal_partition;
    double normal_thresh;
} vorbis_info_psy;

typedef struct vorbis_look_psy {
    int              n;
    vorbis_info_psy* vi;
    float***         tonecurves;
    float**          noiseoffset;
    float*           ath;
    long*            octave;
    long*            bark;
    long             firstoc;
    long             shiftoc;
    int              eighth_octave_lines;
    int              total_octave_lines;
    long             rate;
} vorbis_look_psy;

extern "C" void __fastcall _vp_noise_normalize_sort(vorbis_look_psy* p, float* magnitudes, int* sortedindex)
{
    int i, j, n = p->n;
    vorbis_info_psy* vi = p->vi;
    int partition = vi->normal_partition;
    float** work = (float**)_alloca(sizeof(*work) * partition);
    int start = vi->normal_start;

    for (j = start; j < n; j += partition) {
        if (j + partition > n) partition = n - j;
        for (i = 0; i < partition; i++) work[i] = magnitudes + i + j;
        qsort(work, partition, sizeof(*work), apsort);
        for (i = 0; i < partition; i++) {
            sortedindex[i + j - start] = work[i] - magnitudes;
        }
    }
}