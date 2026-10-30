// Behaviour test for 0x00C8FA60 vorbis_analysis_wrote (libvorbis block.c): vals > 0 advances pcm_current (OV_EINVAL past
// the storage) and reverse-extrapolates once enough is buffered; vals <= 0 marks eof, pads three long blocks and LPC-
// extrapolates each channel (zero fill when eofflag <= 64). Callees are stand-ins that record their calls.
#include <cstdio>
#include <cstring>

struct vorbis_info { int version; int channels; void* codec_setup; };
struct codec_setup_info { int blocksizes[2]; };
struct vorbis_dsp_state { vorbis_info* vi; float** pcm; int pcm_storage; int pcm_current; int preextrapolate; int eofflag; int centerW; };

static int g_buffer = 0, g_from = 0, g_predict = 0, g_helper = 0, g_lastN = 0;
static void vorbis_analysis_buffer(vorbis_dsp_state*, int vals) { g_buffer = vals; }
static float vorbis_lpc_from_data(float*, float*, int n, int) { ++g_from; g_lastN = n; return 0; }
static void vorbis_lpc_predict(float*, float*, int, float* data, long n) { ++g_predict; for (long i = 0; i < n; ++i) data[i] = 7; }
static void _preextrapolate_helper(vorbis_dsp_state* v) { ++g_helper; v->preextrapolate = 1; }

static int vorbis_analysis_wrote(vorbis_dsp_state* v, int vals)
{
    vorbis_info* vi = v->vi;
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    if (vals <= 0) {
        int order = 32;
        int i;
        float lpc[32];
        if (!v->preextrapolate)
            _preextrapolate_helper(v);
        vorbis_analysis_buffer(v, ci->blocksizes[1] * 3);
        v->eofflag = v->pcm_current;
        v->pcm_current += ci->blocksizes[1] * 3;
        for (i = 0; i < vi->channels; i++) {
            if (v->eofflag > order * 2) {
                long n = v->eofflag;
                if (n > ci->blocksizes[1]) n = ci->blocksizes[1];
                vorbis_lpc_from_data(v->pcm[i] + v->eofflag - n, lpc, n, order);
                vorbis_lpc_predict(lpc, v->pcm[i] + v->eofflag - order, order, v->pcm[i] + v->eofflag, v->pcm_current - v->eofflag);
            } else {
                memset(v->pcm[i] + v->eofflag, 0, (v->pcm_current - v->eofflag) * sizeof(*v->pcm[i]));
            }
        }
    } else {
        if (v->pcm_current + vals > v->pcm_storage)
            return -131;
        v->pcm_current += vals;
        if (!v->preextrapolate && v->pcm_current - v->centerW > ci->blocksizes[1])
            _preextrapolate_helper(v);
    }
    return 0;
}

int main()
{
    static float ch0[1000], ch1[1000];
    float* pcm[2] = { ch0, ch1 };
    codec_setup_info ci = { { 32, 100 } };
    vorbis_info vi = { 0, 2, &ci };
    vorbis_dsp_state v = { &vi, pcm, 1000, 0, 0, 0, 0 };
    for (int i = 0; i < 1000; ++i) ch0[i] = ch1[i] = 1;
    if (vorbis_analysis_wrote(&v, 2000) != -131 || v.pcm_current != 0) { std::printf("00c8fa60_TEST FAIL\n"); return 1; }
    if (vorbis_analysis_wrote(&v, 50) != 0 || v.pcm_current != 50 || g_helper != 0) { std::printf("00c8fa60_TEST FAIL\n"); return 1; }
    if (vorbis_analysis_wrote(&v, 0) != 0 || g_helper != 1 || g_buffer != 300 || v.eofflag != 50 || v.pcm_current != 350) { std::printf("00c8fa60_TEST FAIL\n"); return 1; }
    if (g_from != 0 || ch0[50] != 0 || ch1[349] != 0 || ch0[49] != 1 || ch0[350] != 1) { std::printf("00c8fa60_TEST FAIL\n"); return 1; }
    vorbis_dsp_state w = { &vi, pcm, 1000, 0, 0, 0, 0 };
    if (vorbis_analysis_wrote(&w, 150) != 0 || g_helper != 2 || w.preextrapolate != 1) { std::printf("00c8fa60_TEST FAIL\n"); return 1; }
    if (vorbis_analysis_wrote(&w, -1) != 0 || g_helper != 2 || g_from != 2 || g_predict != 2 || g_lastN != 100 || ch1[150] != 7 || ch1[449] != 7) {
        std::printf("00c8fa60_TEST FAIL\n"); return 1;
    }
    std::printf("00c8fa60_TEST PASS\n");
    return 0;
}