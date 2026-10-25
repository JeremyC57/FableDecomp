// Behaviour test for 0x00C8B9D0 ov_time_tell (vorbisfile): copy of the model and the logic, with stand-in callees.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct OggVorbis_File {
    void* datasource; int seekable; int links; ogg_int64_t* pcmlengths; vorbis_info* vi;
    ogg_int64_t pcm_offset; int ready_state;
};
#define OV_EINVAL -131
#define OPENED 2
ogg_int64_t ov_pcm_total(OggVorbis_File* vf, int i)        // stand-in for 0x00C8AA80 (i < 0: whole file)
{
    ogg_int64_t acc = 0; int k;
    if (i >= 0) return vf->pcmlengths[i * 2 + 1];
    for (k = 0; k < vf->links; k++) acc += vf->pcmlengths[k * 2 + 1];
    return acc;
}
double ov_time_total(OggVorbis_File* vf, int i)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable || i >= vf->links) return (OV_EINVAL);
    if (i < 0) {
        double acc = 0; int k;
        for (k = 0; k < vf->links; k++) acc += ov_time_total(vf, k);
        return (acc);
    }
    return ((double)(vf->pcmlengths[i * 2 + 1]) / vf->vi[i].rate);
}
double ov_time_tell(OggVorbis_File* vf)
{
    int link = 0;
    ogg_int64_t pcm_total = 0;
    double time_total = 0.f;

    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (vf->seekable) {
        pcm_total = ov_pcm_total(vf, -1);
        time_total = ov_time_total(vf, -1);
        for (link = vf->links - 1; link >= 0; link--) {
            pcm_total -= vf->pcmlengths[link * 2 + 1];
            time_total -= ov_time_total(vf, link);
            if (vf->pcm_offset >= pcm_total) break;
        }
    }
    return ((double)time_total + (double)(vf->pcm_offset - pcm_total) / vf->vi[link].rate);
}
static int near_(double a, double b) { double d = a - b; return d < 1e-9 && d > -1e-9; }
int main()
{
    ogg_int64_t pcm[4] = { 0, 44100, 0, 22050 };
    vorbis_info vi[2]; vi[0].rate = 44100; vi[1].rate = 22050;
    OggVorbis_File vf; vf.datasource = 0; vf.seekable = 1; vf.links = 2; vf.pcmlengths = pcm; vf.vi = vi; vf.ready_state = 4;
    int ok = 1;
    vf.pcm_offset = 50000;                                        // inside link 1: 1 s + 5900 / 22050
    ok &= near_(ov_time_tell(&vf), 1.0 + 5900.0 / 22050.0);
    vf.pcm_offset = 22050;                                        // inside link 0: 0.5 s
    ok &= near_(ov_time_tell(&vf), 0.5);
    vf.seekable = 0; vf.pcm_offset = 4410;                        // unseekable: link 0 rate
    ok &= near_(ov_time_tell(&vf), 0.1);
    vf.ready_state = 1; ok &= near_(ov_time_tell(&vf), -131.0);
    if (!ok) { std::printf("00c8b9d0_TEST FAIL\n"); return 1; }
    std::printf("00c8b9d0_TEST PASS\n");
    return 0;
}