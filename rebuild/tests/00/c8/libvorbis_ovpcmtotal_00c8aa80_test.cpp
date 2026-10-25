// Behaviour test for 00c8aa80: ov_pcm_total -- per-link PCM length, the sum for i < 0, OV_EINVAL edges
#include <cstdio>

#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
typedef struct OggVorbis_File { int seekable; int links; ogg_int64_t* pcmlengths; int ready_state; } OggVorbis_File;
#define OV_EINVAL -131
#define OPENED 2

ogg_int64_t ov_pcm_total(OggVorbis_File* vf, int i)
{
    if (vf->ready_state < OPENED) return (OV_EINVAL);
    if (!vf->seekable || i >= vf->links) return (OV_EINVAL);
    if (i < 0) {
        ogg_int64_t acc = 0;
        int i;
        for (i = 0; i < vf->links; i++)
            acc += ov_pcm_total(vf, i);
        return (acc);
    } else {
        return (vf->pcmlengths[i * 2 + 1]);
    }
}

int main()
{
    ogg_int64_t lengths[6] = { 0, 1000, 5, ((ogg_int64_t)1 << 32), 7, 24 };
    OggVorbis_File vf;
    vf.seekable = 1; vf.links = 3; vf.pcmlengths = lengths; vf.ready_state = 1;
    int ok = ov_pcm_total(&vf, 0) == OV_EINVAL;
    vf.ready_state = 2;
    ok = ok && ov_pcm_total(&vf, 1) == ((ogg_int64_t)1 << 32) && ov_pcm_total(&vf, 2) == 24
        && ov_pcm_total(&vf, -1) == 1000 + ((ogg_int64_t)1 << 32) + 24 && ov_pcm_total(&vf, 3) == OV_EINVAL;
    vf.seekable = 0;
    ok = ok && ov_pcm_total(&vf, -1) == OV_EINVAL;
    if (!ok) { std::printf("00c8aa80_TEST FAIL\n"); return 1; }
    std::printf("00c8aa80_TEST PASS\n");
    return 0;
}