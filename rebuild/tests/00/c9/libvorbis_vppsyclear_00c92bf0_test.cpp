// Behaviour test for 0x00C92BF0 libvorbis _vp_psy_clear (copy of the model + logic, counting frees).
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct vorbis_look_psy {
    int n; void* vi; float*** tonecurves; float** noiseoffset; float* ath; long* octave; long* bark;
    long firstoc, shiftoc; int eighth_octave_lines, total_octave_lines; long rate;
};
static int g_frees;
static void count_free(void* p) { g_frees++; std::free(p); }
static void psy_clear(vorbis_look_psy* p)
{
    int i, j;
    if (p) {
        if (p->ath) count_free(p->ath);
        if (p->octave) count_free(p->octave);
        if (p->bark) count_free(p->bark);
        if (p->tonecurves) {
            for (i = 0; i < 17; i++) {
                for (j = 0; j < 8; j++) count_free(p->tonecurves[i][j]);
                count_free(p->tonecurves[i]);
            }
            count_free(p->tonecurves);
        }
        if (p->noiseoffset) {
            for (i = 0; i < 3; i++) count_free(p->noiseoffset[i]);
            count_free(p->noiseoffset);
        }
        std::memset(p, 0, sizeof(*p));
    }
}
int main()
{
    int i, j, ok = 1;
    vorbis_look_psy p;
    p.n = 128; p.vi = &p; p.ath = (float*)std::malloc(16); p.octave = (long*)std::malloc(16); p.bark = 0;
    p.tonecurves = (float***)std::malloc(17 * sizeof(float**));
    for (i = 0; i < 17; i++) {
        p.tonecurves[i] = (float**)std::malloc(8 * sizeof(float*));
        for (j = 0; j < 8; j++) p.tonecurves[i][j] = (float*)std::malloc(8);
    }
    p.noiseoffset = (float**)std::malloc(3 * sizeof(float*));
    for (i = 0; i < 3; i++) p.noiseoffset[i] = (float*)std::malloc(8);
    p.firstoc = 1; p.shiftoc = 2; p.eighth_octave_lines = 8; p.total_octave_lines = 9; p.rate = 44100;
    g_frees = 0;
    psy_clear(&p);
    if (g_frees != 2 + 17 * 8 + 17 + 1 + 3 + 1) ok = 0;      /* bark was null */
    if (p.n || p.vi || p.tonecurves || p.noiseoffset || p.ath || p.rate || p.total_octave_lines) ok = 0;
    g_frees = 0;
    psy_clear(0);                                             /* null look: nothing */
    psy_clear(&p);                                            /* cleared look: nothing to free */
    if (g_frees != 0) ok = 0;
    if (!ok) { std::printf("00c92bf0_TEST FAIL\n"); return 1; }
    std::printf("00c92bf0_TEST PASS\n");
    return 0;
}