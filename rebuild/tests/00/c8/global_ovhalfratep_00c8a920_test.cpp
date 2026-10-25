// Behaviour test for 00c8a920: ov_halfrate_p -- OV_EINVAL without decoder info, else the codec's halfrate flag
#include <cstdio>
#include <cstring>
struct vorbis_info { int flag; };
struct OggVorbis_File { unsigned char head[0x48]; vorbis_info* vi; };
static int vorbis_synthesis_halfrate_p(vorbis_info* vi) { return vi->flag; }
static int ov_halfrate_p(OggVorbis_File* vf) { if (vf->vi == 0) return -131; return vorbis_synthesis_halfrate_p(vf->vi); }
int main()
{
    OggVorbis_File vf; std::memset(&vf, 0, sizeof vf); vorbis_info vi; vi.flag = 1;
    int ok = ov_halfrate_p(&vf) == -131; vf.vi = &vi; ok = ok && ov_halfrate_p(&vf) == 1;
    std::printf(ok ? "00c8a920_TEST PASS\n" : "00c8a920_TEST FAIL\n");
    return ok ? 0 : 1;
}
