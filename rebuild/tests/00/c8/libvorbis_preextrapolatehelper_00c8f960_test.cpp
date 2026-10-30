// Behaviour test for libvorbis block.c _preextrapolate_helper (0x00C8F960): reverse each channel, LPC-predict the head, reverse back.
#include <cstdio>
struct vorbis_info { int version; int channels; };
struct vorbis_dsp_state { vorbis_info* vi; float** pcm; int pcm_current; int preextrapolate; long centerW; };
static int g_from, g_pred;
static float vorbis_lpc_from_data(float*, float*, int, int) { ++g_from; return 0.0f; }
static void vorbis_lpc_predict(float*, float*, int, float* data, long n) { ++g_pred; for (long i = 0; i < n; i++) data[i] = -1.0f; }
static void _preextrapolate_helper(vorbis_dsp_state* v)
{
    int i; int order = 32;
    static float lpc[32], work[1024];
    long j;
    v->preextrapolate = 1;
    if (v->pcm_current - v->centerW > order * 2) {
        for (i = 0; i < v->vi->channels; i++) {
            for (j = 0; j < v->pcm_current; j++) work[j] = v->pcm[i][v->pcm_current - j - 1];
            vorbis_lpc_from_data(work, lpc, v->pcm_current - v->centerW, order);
            vorbis_lpc_predict(lpc, work + v->pcm_current - v->centerW - order, order, work + v->pcm_current - v->centerW, v->centerW);
            for (j = 0; j < v->pcm_current; j++) v->pcm[i][v->pcm_current - j - 1] = work[j];
        }
    }
}
int main()
{
    static float ch0[200], ch1[200]; float* pcm[2] = { ch0, ch1 };
    for (int k = 0; k < 200; k++) { ch0[k] = (float)k; ch1[k] = (float)(2 * k); }
    vorbis_info vi = { 0, 2 };
    vorbis_dsp_state v = { &vi, pcm, 50, 0, 10 };
    _preextrapolate_helper(&v);                 /* 50 - 10 <= 64: only the flag */
    if (v.preextrapolate != 1 || g_from != 0 || ch0[0] != 0.0f) { std::printf("00c8f960_TEST FAIL\n"); return 1; }
    v.pcm_current = 200; v.centerW = 20;
    _preextrapolate_helper(&v);
    if (g_from != 2 || g_pred != 2 || ch0[0] != -1.0f || ch0[19] != -1.0f || ch0[20] != 20.0f || ch1[199] != 398.0f)
    { std::printf("00c8f960_TEST FAIL\n"); return 1; }
    std::printf("00c8f960_TEST PASS\n");
    return 0;
}