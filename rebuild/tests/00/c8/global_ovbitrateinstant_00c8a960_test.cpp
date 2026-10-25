// Behaviour test for 00c8a960: ov_bitrate_instant -- bits/samples*rate rounded, then reset the trackers; OV_EINVAL / OV_FALSE edges
#include <cstdio>

typedef struct vorbis_info { int version; int channels; long rate; long pad[5]; } vorbis_info;
typedef struct OggVorbis_File {
    int seekable; vorbis_info* vi; int ready_state; int current_link; double bittrack; double samptrack;
} OggVorbis_File;
#define OV_FALSE  -1
#define OV_EINVAL -131
#define OPENED 2

long ov_bitrate_instant(OggVorbis_File* vf)
{
    int link = (vf->seekable ? vf->current_link : 0);
    long ret;
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (vf->samptrack == 0) return (OV_FALSE);
    ret = (long)(vf->bittrack / vf->samptrack * vf->vi[link].rate + .5);
    vf->bittrack = 0.f;
    vf->samptrack = 0.f;
    return (ret);
}

int main()
{
    vorbis_info vi[2];
    vi[0].rate = 44100; vi[1].rate = 22050;
    OggVorbis_File vf;
    vf.seekable = 1; vf.vi = vi; vf.ready_state = 1; vf.current_link = 1; vf.bittrack = 1000.0; vf.samptrack = 441.0;
    int ok = ov_bitrate_instant(&vf) == OV_EINVAL;
    vf.ready_state = 4;
    long r = ov_bitrate_instant(&vf);            // 1000 / 441 * 22050 = 50000
    ok = ok && r == 50000 && vf.bittrack == 0.0 && vf.samptrack == 0.0;
    ok = ok && ov_bitrate_instant(&vf) == OV_FALSE;
    vf.seekable = 0; vf.bittrack = 3.0; vf.samptrack = 44100.0;   // link 0: 3 bits/s -> 3
    ok = ok && ov_bitrate_instant(&vf) == 3;
    if (!ok) { std::printf("00c8a960_TEST FAIL\n"); return 1; }
    std::printf("00c8a960_TEST PASS\n");
    return 0;
}