// Behaviour test for libvorbis vorbis_synthesis_blockin (0x00C8FF30): the 1.0.1 block.c body over minimal structures --
// the OV_EINVAL guards, the first-block state (pcm_returned == -1), short/short overlap-add with the window, the copy
// section, centerW toggling, granulepos/sample_count tracking and the end-of-stream trim.
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
#include <stdint.h>
typedef int64_t ogg_int64_t;
#endif
#define OV_EINVAL -131
struct codec_setup_info { long blocksizes[2]; int halfrate_flag; };
struct vorbis_info { int channels; codec_setup_info* codec_setup; };
struct private_state { int window[2]; ogg_int64_t sample_count; };
struct vorbis_dsp_state {
    vorbis_info* vi; float** pcm; int pcm_current, pcm_returned, eofflag; long lW, W, nW, centerW;
    ogg_int64_t granulepos, sequence, glue_bits, time_bits, floor_bits, res_bits; void* backend_state;
};
struct vorbis_block { float** pcm; long lW, W, nW; int eofflag; ogg_int64_t granulepos, sequence; long glue_bits, time_bits, floor_bits, res_bits; };
static float g_win[2][64];
static float* _vorbis_window_get(int n) { return g_win[n]; }
static int vorbis_synthesis_blockin(vorbis_dsp_state* v, vorbis_block* vb)
{
    vorbis_info* vi = v->vi; codec_setup_info* ci = vi->codec_setup; private_state* b = (private_state*)v->backend_state;
    int hs = ci->halfrate_flag; int i, j;
    if (!vb) return (OV_EINVAL);
    if (v->pcm_current > v->pcm_returned && v->pcm_returned != -1) return (OV_EINVAL);
    v->lW = v->W; v->W = vb->W; v->nW = -1;
    if ((v->sequence == -1) || (v->sequence + 1 != vb->sequence)) { v->granulepos = -1; b->sample_count = -1; }
    v->sequence = vb->sequence;
    if (vb->pcm) {
        int n = ci->blocksizes[v->W] >> (hs + 1); int n0 = ci->blocksizes[0] >> (hs + 1); int n1 = ci->blocksizes[1] >> (hs + 1);
        int thisCenter, prevCenter;
        v->glue_bits += vb->glue_bits; v->time_bits += vb->time_bits; v->floor_bits += vb->floor_bits; v->res_bits += vb->res_bits;
        if (v->centerW) { thisCenter = n1; prevCenter = 0; } else { thisCenter = 0; prevCenter = n1; }
        for (j = 0; j < vi->channels; j++) {
            if (v->lW) {
                if (v->W) { float* w = _vorbis_window_get(b->window[1] - hs); float* pcm = v->pcm[j] + prevCenter; float* p = vb->pcm[j];
                    for (i = 0; i < n1; i++) pcm[i] = pcm[i] * w[n1 - i - 1] + p[i] * w[i];
                } else { float* w = _vorbis_window_get(b->window[0] - hs); float* pcm = v->pcm[j] + prevCenter + n1 / 2 - n0 / 2; float* p = vb->pcm[j];
                    for (i = 0; i < n0; i++) pcm[i] = pcm[i] * w[n0 - i - 1] + p[i] * w[i]; }
            } else {
                if (v->W) { float* w = _vorbis_window_get(b->window[0] - hs); float* pcm = v->pcm[j] + prevCenter; float* p = vb->pcm[j] + n1 / 2 - n0 / 2;
                    for (i = 0; i < n0; i++) pcm[i] = pcm[i] * w[n0 - i - 1] + p[i] * w[i];
                    for (; i < n1 / 2 + n0 / 2; i++) pcm[i] = p[i];
                } else { float* w = _vorbis_window_get(b->window[0] - hs); float* pcm = v->pcm[j] + prevCenter; float* p = vb->pcm[j];
                    for (i = 0; i < n0; i++) pcm[i] = pcm[i] * w[n0 - i - 1] + p[i] * w[i]; }
            }
            { float* pcm = v->pcm[j] + thisCenter; float* p = vb->pcm[j] + n; for (i = 0; i < n; i++) pcm[i] = p[i]; }
        }
        if (v->centerW) v->centerW = 0; else v->centerW = n1;
        if (v->pcm_returned == -1) { v->pcm_returned = thisCenter; v->pcm_current = thisCenter; }
        else { v->pcm_returned = prevCenter; v->pcm_current = prevCenter + ((ci->blocksizes[v->lW] / 4 + ci->blocksizes[v->W] / 4) >> hs); }
    }
    if (b->sample_count == -1) b->sample_count = 0;
    else b->sample_count += ci->blocksizes[v->lW] / 4 + ci->blocksizes[v->W] / 4;
    if (v->granulepos == -1) {
        if (vb->granulepos != -1) {
            v->granulepos = vb->granulepos;
            if (b->sample_count > v->granulepos) {
                if (vb->eofflag) v->pcm_current -= (int)((b->sample_count - v->granulepos) >> hs);
                else { v->pcm_returned += (int)((b->sample_count - v->granulepos) >> hs); if (v->pcm_returned > v->pcm_current) v->pcm_returned = v->pcm_current; }
            }
        }
    } else {
        v->granulepos += ci->blocksizes[v->lW] / 4 + ci->blocksizes[v->W] / 4;
        if (vb->granulepos != -1 && v->granulepos != vb->granulepos) {
            if (v->granulepos > vb->granulepos) { long extra = (long)(v->granulepos - vb->granulepos); if (extra) if (vb->eofflag) v->pcm_current -= extra >> hs; }
            v->granulepos = vb->granulepos;
        }
    }
    if (vb->eofflag) v->eofflag = 1;
    return (0);
}
int main()
{
    for (int i = 0; i < 64; i++) { g_win[0][i] = (i + 1) / 8.f; g_win[1][i] = 1.f; }
    codec_setup_info ci = { { 16, 64 }, 0 };   // n0 = 8, n1 = 32
    vorbis_info vi = { 1, &ci };
    private_state ps = { { 0, 1 }, 0 };
    static float dsp[128]; float* dpcm[1] = { dsp };
    vorbis_dsp_state v; std::memset(&v, 0, sizeof(v)); v.vi = &vi; v.pcm = dpcm; v.backend_state = &ps; v.pcm_returned = -1; v.sequence = -1;
    float blk[16]; for (int i = 0; i < 16; i++) blk[i] = (float)(i + 1); float* bpcm[1] = { blk };
    vorbis_block vb; std::memset(&vb, 0, sizeof(vb)); vb.pcm = bpcm; vb.W = 0; vb.sequence = 0; vb.granulepos = -1; vb.glue_bits = 5;
    int bad = 0;
    if (vorbis_synthesis_blockin(&v, 0) != OV_EINVAL) bad = 1;
    // first short block: overlap into prevCenter = n1 = 32 (old contents zero), copy section to thisCenter = 0
    if (vorbis_synthesis_blockin(&v, &vb) != 0) bad = 2;
    for (int i = 0; i < 8; i++) if (std::fabs(dsp[32 + i] - blk[i] * g_win[0][i]) > 1e-6f || dsp[i] != blk[8 + i]) bad = 3;
    if (v.centerW != 32 || v.pcm_returned != 0 || v.pcm_current != 0 || ps.sample_count != 0 || v.granulepos != -1 || v.glue_bits != 5) bad = 4;
    if (vorbis_synthesis_blockin(&v, &vb) != 0) bad = 5;                 // pcm_current (0) <= pcm_returned: accepted
    // second short block (sequence restarts: same 0, so sample_count reset again); now thisCenter 32, prevCenter 0
    if (v.centerW != 0 || v.pcm_returned != 0 || v.pcm_current != 0 + (16 / 4 + 16 / 4)) bad = 6;
    for (int i = 0; i < 8; i++) {
        float old = blk[8 + i];
        if (std::fabs(dsp[i] - (old * g_win[0][8 - i - 1] + blk[i] * g_win[0][i])) > 1e-5f) bad = 7;
    }
    v.pcm_returned = v.pcm_current;
    vb.sequence = 1; vb.granulepos = 100; vb.eofflag = 1;               // in sequence; granule lower than counted -> trim end
    ps.sample_count = 200; v.granulepos = -1;
    if (vorbis_synthesis_blockin(&v, &vb) != 0) bad = 8;
    if (v.granulepos != 100 || ps.sample_count != 208 || v.pcm_returned != 32 || v.pcm_current != 32 + 8 - 108 || !v.eofflag) bad = 9;
    if (bad) { std::printf("00c8ff30_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c8ff30_TEST PASS\n"); return 0;
}