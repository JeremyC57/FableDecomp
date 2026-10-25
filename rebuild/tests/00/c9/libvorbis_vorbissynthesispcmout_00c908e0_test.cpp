// Behaviour test for 0x00C908E0 libvorbis vorbis_synthesis_pcmout (copy of the model + logic).
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct vorbis_dsp_state {
    int analysisp; vorbis_info* vi; float** pcm; float** pcmret; int pcm_storage; int pcm_current; int pcm_returned;
    int preextrapolate; int eofflag; long lW, W, nW, centerW; i64 granulepos, sequence, glue_bits, time_bits, floor_bits, res_bits;
    void* backend_state;
};
static int pcmout(vorbis_dsp_state* v, float*** pcm)
{
    vorbis_info* vi = v->vi;
    if (v->pcm_returned > -1 && v->pcm_returned < v->pcm_current) {
        if (pcm) {
            int i;
            for (i = 0; i < vi->channels; i++) v->pcmret[i] = v->pcm[i] + v->pcm_returned;
            *pcm = v->pcmret;
        }
        return v->pcm_current - v->pcm_returned;
    }
    return 0;
}
int main()
{
    float c0[16], c1[16]; float* chans[2] = { c0, c1 }; float* ret[2] = { 0, 0 };
    vorbis_info vi = { 0, 2, 44100, 0, 0, 0, 0, 0 };
    vorbis_dsp_state v = { 0 };
    v.vi = &vi; v.pcm = chans; v.pcmret = ret; v.pcm_current = 10; v.pcm_returned = 3;
    float** out = 0;
    int ok = 1;
    if (pcmout(&v, &out) != 7 || out != ret || ret[0] != c0 + 3 || ret[1] != c1 + 3) ok = 0;
    ret[0] = ret[1] = 0;
    if (pcmout(&v, 0) != 7 || ret[0] != 0) ok = 0;            /* no pcm pointer: count only */
    v.pcm_returned = -1;
    if (pcmout(&v, &out) != 0) ok = 0;                         /* not primed */
    v.pcm_returned = 10;
    if (pcmout(&v, &out) != 0) ok = 0;                         /* nothing left */
    if (!ok) { std::printf("00c908e0_TEST FAIL\n"); return 1; }
    std::printf("00c908e0_TEST PASS\n");
    return 0;
}