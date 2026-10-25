// libvorbis::_vi_psy_free(vorbis_info_psy* i) -- wipe and free a psychoacoustic setup block; NULL is ignored
// Leaf: @_vi_psy_free@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C918A0, 30 bytes, i in ecx, plain `ret`. libvorbis 1.0.1 lib/psy.c.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 447 + agent447 NOTES/LESSONS): Xiph libvorbis 1.0.1 inside libpk
//   ("Xiph.Org libVorbis I 20030909" @ 0x00EBF240), compiled as C with /Gr (`__fastcall`); the free is the MSVCR71
//   import [0x01440154] through _ogg_free, as in the landed agent447 rows (mdct_clear 0x00C9C4C0, res0_free_look 0x00C9FDF0).
// Label: catalog `std::_Cons_val<std::allocator<C3DGeneratorObject>,...>` is propagated; its source modelled the free
//   import as an undefined function-pointer global and the 0x210-byte struct as 0x84 opaque dwords.
// Shape: the shipped source; `rep stosd` x0x84 is the memset of sizeof(vorbis_info_psy).
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

#define P_BANDS 17
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
typedef struct vorbis_info_psy {                   // libvorbis 1.0.1 psy.h, 0x210 bytes
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
    double normal_thresh;                          // +0x208 (8-aligned)
} vorbis_info_psy;

extern "C" void __fastcall _vi_psy_free(vorbis_info_psy* i)
{
    if (i) {
        memset(i, 0, sizeof(*i));
        _ogg_free(i);
    }
}
