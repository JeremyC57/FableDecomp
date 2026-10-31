// Behaviour test for libvorbis ov_crosslap 0x00C8C3E0 (vorbisfile.c): degenerate / not-open cases, init errors, then a
// cross-lap: vf1's lapping data (stand-in _ov_getlap) spliced into vf2's lap-out buffer with the shorter window squared.
#include <cstdio>
#include <cstdlib>

#define OV_EINVAL -131
#define OPENED 2
struct vorbis_info { int version; int channels; int blocksize0; };
struct File { int ready_state; int init_err; int prime_err; int halfrate; vorbis_info vi; float window[8]; float* lap[2]; };
static int g_dumps;
static int _ov_initset(File* vf) { return vf->init_err; }
static int _ov_initprime(File* vf) { return vf->prime_err; }
static vorbis_info* ov_info(File* vf, int) { return &vf->vi; }
static int ov_halfrate_p(File* vf) { return vf->halfrate; }
static int vorbis_info_blocksize(vorbis_info* vi, int) { return vi->blocksize0; }
static float* vorbis_window(File* vf, int) { return vf->window; }
static void _ov_getlap(File*, vorbis_info*, File*, float** lappcm, int n) { for (int c = 0; c < 2; ++c) for (int i = 0; i < n; ++i) lappcm[c][i] = 10.0f; }
static void vorbis_synthesis_lapout(File* vf, float*** pcm) { *pcm = vf->lap; }
static void _analysis_output_always(const char*, int, float*, int, int, int, int) { ++g_dumps; }
static void _ov_splice(float** pcm, float** lappcm, int n1, int n2, int ch1, int ch2, float* w1, float* w2)
{
    int i, j; float* w = w1; int n = n1;
    if (n1 > n2) { n = n2; w = w2; }
    for (j = 0; j < ch1 && j < ch2; j++) {
        float* s = lappcm[j]; float* d = pcm[j];
        for (i = 0; i < n; i++) { float wd = w[i] * w[i]; float ws = 1.0f - wd; d[i] = d[i] * wd + s[i] * ws; }
    }
    for (; j < ch2; j++) { float* d = pcm[j]; for (i = 0; i < n; i++) d[i] = d[i] * w[i] * w[i]; }
}
static int ov_crosslap(File* vf1, File* vf2)
{
    if (vf1 == vf2) return 0;
    if (vf1->ready_state < OPENED) return OV_EINVAL;
    if (vf2->ready_state < OPENED) return OV_EINVAL;
    int ret = _ov_initset(vf1); if (ret) return ret;
    ret = _ov_initprime(vf2); if (ret) return ret;
    vorbis_info* vi1 = ov_info(vf1, -1); vorbis_info* vi2 = ov_info(vf2, -1);
    int hs1 = ov_halfrate_p(vf1), hs2 = ov_halfrate_p(vf2);
    float** lappcm = (float**)std::malloc(sizeof(*lappcm) * vi1->channels);
    int n1 = vorbis_info_blocksize(vi1, 0) >> (1 + hs1);
    int n2 = vorbis_info_blocksize(vi2, 0) >> (1 + hs2);
    float* w1 = vorbis_window(vf1, 0); float* w2 = vorbis_window(vf2, 0);
    for (int i = 0; i < vi1->channels; i++) lappcm[i] = (float*)std::malloc(sizeof(**lappcm) * n1);
    _ov_getlap(vf1, vi1, vf1, lappcm, n1);
    float** pcm; vorbis_synthesis_lapout(vf2, &pcm);
    _analysis_output_always("pcmL", 0, pcm[0], n1 * 2, 0, 0, 0);
    _analysis_output_always("pcmR", 0, pcm[1], n1 * 2, 0, 0, 0);
    _ov_splice(pcm, lappcm, n1, n2, vi1->channels, vi2->channels, w1, w2);
    for (int i = 0; i < vi1->channels; i++) std::free(lappcm[i]);
    std::free(lappcm);
    return 0;
}
int main()
{
    static float l0[8], l1[8];
    File a, b;
    a.ready_state = 4; a.init_err = 0; a.prime_err = 0; a.halfrate = 0; a.vi.channels = 2; a.vi.blocksize0 = 16;
    b = a; b.vi.blocksize0 = 8; b.lap[0] = l0; b.lap[1] = l1;
    for (int i = 0; i < 8; ++i) { a.window[i] = 1.0f; b.window[i] = 0.5f; l0[i] = 2.0f; l1[i] = 4.0f; }
    bool ok = ov_crosslap(&a, &a) == 0;
    a.ready_state = 1; ok = ok && ov_crosslap(&a, &b) == OV_EINVAL; a.ready_state = 4;
    b.ready_state = 0; ok = ok && ov_crosslap(&a, &b) == OV_EINVAL; b.ready_state = 4;
    a.init_err = -3; ok = ok && ov_crosslap(&a, &b) == -3; a.init_err = 0;
    b.prime_err = -137; ok = ok && ov_crosslap(&a, &b) == -137; b.prime_err = 0;
    g_dumps = 0;
    ok = ok && ov_crosslap(&a, &b) == 0 && g_dumps == 2;
    // n1 = 8, n2 = 4 -> n = 4 with w2 = 0.5: d = d*0.25 + 10*0.75
    ok = ok && l0[0] == 8.0f && l1[3] == 8.5f && l0[4] == 2.0f;
    if (!ok) { std::printf("00c8c3e0_TEST FAIL\n"); return 1; }
    std::printf("00c8c3e0_TEST PASS\n");
    return 0;
}