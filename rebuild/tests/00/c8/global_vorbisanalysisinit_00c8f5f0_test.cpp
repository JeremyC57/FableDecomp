// Behaviour test for libvorbis block.c _vds_shared_init (0x00C8F200) and its callers vorbis_analysis_init (0x00C8F5F0) /
// vorbis_synthesis_init (0x00C8FF00): decode/encode set-up of the DSP state (reduced model: windows, PCM storage, centre).
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int ilog2(unsigned int v) { int ret = 0; if (v) --v; while (v) { ret++; v >>= 1; } return ret; }
struct ci_t { long blocksizes[2]; int modes, halfrate_flag; };
struct vi_t { int channels; long rate; ci_t* codec_setup; };
struct priv_t { int window[2]; int modebits; void* psy_g_look; void* ve; };
struct v_t { vi_t* vi; float** pcm; int pcm_storage, pcm_current, centerW, lW, W, analysisp, restarted; priv_t* backend_state; };
static int _vds_shared_init(v_t* v, vi_t* vi, int encp)
{
    ci_t* ci = vi->codec_setup;
    if (ci == 0) return 1;
    std::memset(v, 0, sizeof(*v));
    priv_t* b = v->backend_state = (priv_t*)std::calloc(1, sizeof(priv_t));
    v->vi = vi; b->modebits = ilog2(ci->modes);
    b->window[0] = ilog2(ci->blocksizes[0]) - 6; b->window[1] = ilog2(ci->blocksizes[1]) - 6;
    if (encp) v->analysisp = 1;
    v->pcm_storage = ci->blocksizes[1];
    v->pcm = (float**)std::malloc(vi->channels * sizeof(*v->pcm));
    for (int i = 0; i < vi->channels; i++) v->pcm[i] = (float*)std::calloc(v->pcm_storage, sizeof(float));
    v->lW = 0; v->W = 0; v->centerW = ci->blocksizes[1] / 2; v->pcm_current = v->centerW;
    return 0;
}
static int vorbis_analysis_init(v_t* v, vi_t* vi)
{
    if (_vds_shared_init(v, vi, 1)) return 1;
    priv_t* b = v->backend_state; b->psy_g_look = vi; b->ve = std::calloc(1, 0xb4);
    return 0;
}
static int vorbis_synthesis_init(v_t* v, vi_t* vi) { if (_vds_shared_init(v, vi, 0)) return 1; v->restarted = 1; return 0; }
int main()
{
    ci_t ci = { { 256, 2048 }, 2, 0 }; vi_t vi = { 2, 44100, &ci }; v_t v;
    if (vorbis_synthesis_init(&v, &vi) != 0 || !v.restarted || v.analysisp || v.pcm_storage != 2048 || v.centerW != 1024
        || v.pcm_current != 1024 || v.backend_state->window[0] != 2 || v.backend_state->window[1] != 5 || v.backend_state->modebits != 1)
    { std::printf("00c8f5f0_TEST FAIL\n"); return 1; }
    if (vorbis_analysis_init(&v, &vi) != 0 || !v.analysisp || !v.backend_state->ve) { std::printf("00c8f5f0_TEST FAIL\n"); return 1; }
    vi.codec_setup = 0;
    if (vorbis_analysis_init(&v, &vi) != 1 || vorbis_synthesis_init(&v, &vi) != 1) { std::printf("00c8f5f0_TEST FAIL\n"); return 1; }
    std::printf("00c8f5f0_TEST PASS\n"); return 0;
}