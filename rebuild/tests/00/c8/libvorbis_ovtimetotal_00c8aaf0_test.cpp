// Behaviour test for 0x00C8AAF0 ov_time_total (vorbisfile): copy of the model and the logic.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct OggVorbis_File {
    void* datasource; int seekable; ogg_int64_t offset, end;
    int links; ogg_int64_t* pcmlengths; vorbis_info* vi; int ready_state;
};
#define OV_EINVAL -131
#define OPENED 2
double ov_time_total(OggVorbis_File* vf, int i)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable || i >= vf->links) return (OV_EINVAL);
    if (i < 0) {
        double acc = 0;
        int i;
        for (i = 0; i < vf->links; i++)
            acc += ov_time_total(vf, i);
        return (acc);
    } else {
        return ((double)(vf->pcmlengths[i * 2 + 1]) / vf->vi[i].rate);
    }
}
static int near_(double a, double b) { double d = a - b; return d < 1e-9 && d > -1e-9; }
int main()
{
    ogg_int64_t pcm[4] = { 0, 44100, 0, 11025 };
    vorbis_info vi[2]; vi[0].rate = 44100; vi[1].rate = 22050;
    OggVorbis_File vf; vf.datasource = 0; vf.seekable = 1; vf.links = 2; vf.pcmlengths = pcm; vf.vi = vi; vf.ready_state = 2;
    int ok = 1;
    ok &= near_(ov_time_total(&vf, 0), 1.0);
    ok &= near_(ov_time_total(&vf, 1), 0.5);
    ok &= near_(ov_time_total(&vf, -1), 1.5);
    ok &= near_(ov_time_total(&vf, 2), -131.0);
    vf.seekable = 0; ok &= near_(ov_time_total(&vf, 0), -131.0); vf.seekable = 1;
    vf.ready_state = 1; ok &= near_(ov_time_total(&vf, -1), -131.0);
    if (!ok) { std::printf("00c8aaf0_TEST FAIL\n"); return 1; }
    std::printf("00c8aaf0_TEST PASS\n");
    return 0;
}