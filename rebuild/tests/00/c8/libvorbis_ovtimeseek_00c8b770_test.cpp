// Behaviour test for ov_time_seek (0x00c8b770): vorbisfile time -> pcm translation over a three-link file.
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct vorbis_info { int version; int channels; long rate; };
struct OggVorbis_File {
    int seekable; int links; ogg_int64_t* pcmlengths; vorbis_info* vi; int ready_state;
};
#define OV_EINVAL  -131
#define OV_ENOSEEK -138
#define OPENED     2
static ogg_int64_t g_target = -1;
static ogg_int64_t ov_pcm_total(OggVorbis_File* vf, int i)
{
    if (vf->ready_state < OPENED) return OV_EINVAL;
    if (!vf->seekable || i >= vf->links) return OV_EINVAL;
    if (i < 0) { ogg_int64_t acc = 0; for (int k = 0; k < vf->links; k++) acc += ov_pcm_total(vf, k); return acc; }
    return vf->pcmlengths[i * 2 + 1];
}
static double ov_time_total(OggVorbis_File* vf, int i)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable || i >= vf->links) return (OV_EINVAL);
    if (i < 0) { double acc = 0; for (int k = 0; k < vf->links; k++) acc += ov_time_total(vf, k); return acc; }
    return ((double)(vf->pcmlengths[i * 2 + 1]) / vf->vi[i].rate);
}
static int ov_pcm_seek(OggVorbis_File*, ogg_int64_t pos) { g_target = pos; return 0; }
static int ov_time_seek(OggVorbis_File* vf, double seconds)
{
    int link = -1;
    ogg_int64_t pcm_total = ov_pcm_total(vf, -1);
    double time_total = ov_time_total(vf, -1);
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable) return (OV_ENOSEEK);
    if (seconds < 0 || seconds > time_total) return (OV_EINVAL);
    for (link = vf->links - 1; link >= 0; link--) {
        pcm_total -= vf->pcmlengths[link * 2 + 1];
        time_total -= ov_time_total(vf, link);
        if (seconds >= time_total) break;
    }
    {
        ogg_int64_t target = pcm_total + (ogg_int64_t)((seconds - time_total) * vf->vi[link].rate);
        return (ov_pcm_seek(vf, target));
    }
}
int main()
{
    // links: 2 s at 1000 Hz, 1 s at 2000 Hz, 4 s at 500 Hz -> pcm 2000, 2000, 2000; total 7 s, 6000 samples
    ogg_int64_t pl[6] = { 0, 2000, 0, 2000, 0, 2000 };
    vorbis_info vi[3]; vi[0].rate = 1000; vi[1].rate = 2000; vi[2].rate = 500;
    OggVorbis_File vf; vf.seekable = 1; vf.links = 3; vf.pcmlengths = pl; vf.vi = vi; vf.ready_state = OPENED;
    int bad = 0;
    if (ov_time_seek(&vf, 1.5) != 0 || g_target != 1500) bad = 1;            // link 0
    if (ov_time_seek(&vf, 2.5) != 0 || g_target != 3000) bad = 2;            // link 1: 2000 + 0.5*2000
    if (ov_time_seek(&vf, 5.0) != 0 || g_target != 5000) bad = 3;            // link 2: 4000 + 2*500
    if (ov_time_seek(&vf, -1.0) != OV_EINVAL) bad = 4;
    if (ov_time_seek(&vf, 7.5) != OV_EINVAL) bad = 5;
    vf.seekable = 0;
    if (ov_time_seek(&vf, 1.0) != OV_ENOSEEK) bad = 6;
    vf.seekable = 1; vf.ready_state = 1;
    if (ov_time_seek(&vf, 1.0) != OV_EINVAL) bad = 7;
    if (bad) { std::printf("00c8b770_TEST FAIL\n"); return 1; }
    std::printf("00c8b770_TEST PASS\n");
    return 0;
}