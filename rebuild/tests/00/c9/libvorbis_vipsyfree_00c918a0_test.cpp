// Behaviour test for 00c918a0: libvorbis _vi_psy_free -- a block is wiped (all 0x210 bytes) before it is freed; NULL is ignored
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
static int g_freed; static void* g_last; static int g_zero;
static void my_free(void* p) { ++g_freed; g_last = p; const unsigned char* b = (const unsigned char*)p; g_zero = 1; for (unsigned int k = 0; k < sizeof(vorbis_info_psy); ++k) if (b[k]) g_zero = 0; std::free(p); }
static void _vi_psy_free(vorbis_info_psy* info)
{
    if (info) {
        std::memset(info, 0, sizeof(*info));
        my_free(info);
    }
}
int main()
{
    int ok = sizeof(vorbis_info_psy) == 0x210;
    _vi_psy_free(0); ok &= g_freed == 0;
    vorbis_info_psy* p = (vorbis_info_psy*)std::malloc(sizeof *p); std::memset(p, 0x5A, sizeof *p);
    _vi_psy_free(p);
    ok &= g_freed == 1 && g_last == p && g_zero == 1;
    std::printf(ok ? "00c918a0_TEST PASS\n" : "00c918a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
