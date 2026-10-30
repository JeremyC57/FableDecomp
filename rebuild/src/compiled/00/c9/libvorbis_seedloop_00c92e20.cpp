// libvorbis::seed_loop(vorbis_look_psy* p, const float*** curves, const float* f, const float* flr, float* seed, float specmax) -- seed the tone masking vector from each octave group's peak
// Shape (retry1): parked as an "LTCG register argument" gap; seed_loop and its callee seed_curve 0x00C92CA0 are psy.c statics, so the TU holds both as `extern "C" { static __declspec(noinline) __stdcall }` with a _vp_tonemask-shape caller, and VC7.1 /O2 gives retail's private conventions itself (seed_loop: p in EBX, the rest on the stack, ret 14h; seed_curve: ecx/edx + stack).
// Retail: Fable.exe @ 0x00C92E20, 237 bytes. libvorbis 1.0.1 lib/psy.c (static). Grades: 1.
// The bodies are the shipped libvorbis 1.0.1 psy.c source (seed_curve is not graded here).
// Leaf: _seed_loop@24
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
#ifndef P_LEVEL_0
#define P_LEVEL_0 30.f
#endif
#ifndef P_LEVELS
#define P_LEVELS 8
#endif
#ifndef EHMER_OFFSET
#define EHMER_OFFSET 16
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif
extern "C" {
static __declspec(noinline) void __stdcall seed_curve(float* seed, const float** curves, float amp, int oc, int n,
                                                      int linesper, float dBoffset)   // 0x00C92CA0
{
    int i, post1;
    int seedptr;
    const float *posts, *curve;

    int choice = (int)((amp + dBoffset - P_LEVEL_0) * .1f);
    choice = max(choice, 0);
    choice = min(choice, P_LEVELS - 1);
    posts = curves[choice];
    curve = posts + 2;
    post1 = (int)posts[1];
    seedptr = oc + (posts[0] - EHMER_OFFSET) * linesper - (linesper >> 1);

    for (i = posts[0]; i < post1; i++) {
        if (seedptr > 0) {
            float lin = amp + curve[i];
            if (seed[seedptr] < lin) seed[seedptr] = lin;
        }
        seedptr += linesper;
        if (seedptr >= n) break;
    }
}
static __declspec(noinline) void __stdcall seed_loop(vorbis_look_psy* p, const float*** curves, const float* f, const float* flr, float* seed, float specmax)
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

// Same-TU caller (the seed_loop call of psy.c _vp_tonemask), present so the statics get retail's private conventions. Not graded.
void __cdecl vorbis_seed_loop_caller(vorbis_look_psy* p, const float*** curves, const float* f, const float* flr, float* seed, float specmax)
{
    seed_loop(p, curves, f, flr, seed, specmax);
}
}