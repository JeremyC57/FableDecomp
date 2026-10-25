// Behaviour test for 0x00C94190 libvorbis _vp_ampmax_decay (copy of the model + logic).
#include <cstdio>
#include <cstdlib>
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct vorbis_dsp_state { int analysisp; vorbis_info* vi; float** pcm; float** pcmret; int s, c, r, pre, eof; long lW, W, nW, centerW; };
struct psy_global_part { int eighth_octave_lines; float preecho_thresh[7]; float postecho_thresh[7]; float stretch_penalty; float preecho_minenergy; float ampmax_att_per_sec; };
struct codec_setup_part { long blocksizes[2]; psy_global_part psy_g_param; };
static float ampmax_decay(float amp, vorbis_dsp_state* vd)
{
    vorbis_info* vi = vd->vi;
    codec_setup_part* ci = (codec_setup_part*)vi->codec_setup;
    psy_global_part* gi = &ci->psy_g_param;
    int n = ci->blocksizes[vd->W] / 2;
    float secs = (float)n / vi->rate;
    amp += secs * gi->ampmax_att_per_sec;
    if (amp < -9999) amp = -9999;
    return amp;
}
static int near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    codec_setup_part ci = { { 256, 2048 } };
    ci.psy_g_param.ampmax_att_per_sec = -6.f;
    vorbis_info vi = { 0, 2, 44100, 0, 0, 0, 0, &ci };
    vorbis_dsp_state vd = { 0 };
    vd.vi = &vi; vd.W = 1;
    int ok = 1;
    if (!near(ampmax_decay(0.f, &vd), -6.f * 1024.f / 44100.f)) ok = 0;
    vd.W = 0;
    if (!near(ampmax_decay(10.f, &vd), 10.f - 6.f * 128.f / 44100.f)) ok = 0;
    if (ampmax_decay(-9999.5f, &vd) != -9999.f) ok = 0;      /* floored */
    if (!ok) { std::printf("00c94190_TEST FAIL\n"); return 1; }
    std::printf("00c94190_TEST PASS\n");
    return 0;
}