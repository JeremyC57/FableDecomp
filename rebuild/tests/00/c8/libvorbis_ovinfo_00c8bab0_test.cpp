// Behaviour test for 0x00C8BAB0 ov_info (vorbisfile): copy of the model and the logic.
#include <cstdio>
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct vorbis_comment { char** user_comments; int* comment_lengths; int comments; char* vendor; };
struct OggVorbis_File { void* datasource; int seekable; int links; vorbis_info* vi; vorbis_comment* vc; int ready_state; int current_link; };
#define STREAMSET 3
vorbis_info* ov_info(OggVorbis_File* vf, int link)
{
    if (vf->seekable) {
        if (link < 0)
            if (vf->ready_state >= STREAMSET)
                return vf->vi + vf->current_link;
            else
                return vf->vi;
        else
            if (link >= vf->links)
                return 0;
            else
                return vf->vi + link;
    } else {
        return vf->vi;
    }
}
int main()
{
    vorbis_info vi[3]; vorbis_comment vc[3];
    OggVorbis_File vf; vf.datasource = 0; vf.seekable = 1; vf.links = 3; vf.vi = vi; vf.vc = vc; vf.ready_state = 4; vf.current_link = 2;
    int ok = 1;
    ok &= ov_info(&vf, 1) == &vf.vi[1];
    ok &= ov_info(&vf, 3) == 0;
    ok &= ov_info(&vf, -1) == &vf.vi[2];
    vf.ready_state = 2; ok &= ov_info(&vf, -1) == &vf.vi[0];
    vf.seekable = 0; ok &= ov_info(&vf, 2) == &vf.vi[0];
    if (!ok) { std::printf("00c8bab0_TEST FAIL\n"); return 1; }
    std::printf("00c8bab0_TEST PASS\n");
    return 0;
}