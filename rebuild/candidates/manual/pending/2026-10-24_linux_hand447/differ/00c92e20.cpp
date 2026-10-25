// libvorbis::seed_loop(vorbis_look_psy* p, const float*** curves, const float* f, const float* flr, float* seed, float specmax) -- seed the tone masking vector from each octave group's peak
// Grade: DIFFER(235v237) -- LTCG register argument: retail takes p in EBX (never saved, `mov eax,[ebx+4]`) with curves/f/flr/seed/specmax on the stack (`ret 14h`); the callee seed_curve 0x00C92CA0 is also LTCG (ecx/edx + stack); `__fastcall` puts p/curves in ecx/edx
// Retail: Fable.exe @ 0x00C92E20, 237 bytes. libvorbis 1.0.1 lib/psy.c (static). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: seed_loop
#define P_BANDS 17
#define P_NOISECURVES 3
#define NOISE_COMPAND_LEVELS 40
typedef struct vorbis_info_psy {
    int blockflag; float ath_adjatt; float ath_maxatt; float tone_masteratt[P_NOISECURVES];
    float tone_centerboost; float tone_decay; float tone_abs_limit; float toneatt[P_BANDS];
    int noisemaskp; float noisemaxsupp; float noisewindowlo; float noisewindowhi; int noisewindowlomin;
    int noisewindowhimin; int noisewindowfixed; float noiseoff[P_NOISECURVES][P_BANDS];
    float noisecompand[NOISE_COMPAND_LEVELS]; float max_curve_dB; int normal_channel_p; int normal_point_p;
    int normal_start; int normal_partition; double normal_thresh;
} vorbis_info_psy;
typedef struct vorbis_look_psy {
    int n; vorbis_info_psy* vi; float*** tonecurves; float** noiseoffset; float* ath; long* octave; long* bark;
    long firstoc; long shiftoc; int eighth_octave_lines; int total_octave_lines; long rate;
} vorbis_look_psy;
extern "C" void __fastcall seed_curve(float* seed, const float** curves, float amp, int oc, int n, int linesper, float dBoffset);   // 0x00C92CA0
extern "C" void __fastcall seed_loop(vorbis_look_psy* p, const float*** curves, const float* f, const float* flr, float* seed, float specmax)
{
    vorbis_info_psy* vi = p->vi;
    long n = p->n, i;
    float dBoffset = vi->max_curve_dB - specmax;

    for (i = 0; i < n; i++) {
        float max = f[i];
        long oc = p->octave[i];
        while (i + 1 < n && p->octave[i + 1] == oc) {
            i++;
            if (f[i] > max) max = f[i];
        }

        if (max + 6.f > flr[i]) {
            oc = oc >> p->shiftoc;

            if (oc >= P_BANDS) oc = P_BANDS - 1;
            if (oc < 0) oc = 0;

            seed_curve(seed, curves[oc], max, p->octave[i] - p->firstoc, p->total_octave_lines,
                       p->eighth_octave_lines, dBoffset);
        }
    }
}
