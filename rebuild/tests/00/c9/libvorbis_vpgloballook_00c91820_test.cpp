// Behaviour test for 0x00C91820 libvorbis _vp_global_look (copy of the model + logic).
#include <cstdio>
#include <cstdlib>
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct vorbis_info_psy_global {
    int eighth_octave_lines; float preecho_thresh[7]; float postecho_thresh[7]; float stretch_penalty;
    float preecho_minenergy; float ampmax_att_per_sec; int coupling_pkHz[15]; int coupling_pointlimit[2][15];
    int coupling_prepointamp[15]; int coupling_postpointamp[15]; int sliding_lowpass[2][15];
};
struct codec_setup_info {
    long blocksizes[2]; int modes, maps, floors, residues, books, psys; void* mode_param[64]; int map_type[64];
    void* map_param[64]; int floor_type[64]; void* floor_param[64]; int residue_type[64]; void* residue_param[64];
    void* book_param[256]; void* fullbooks; void* psy_param[4]; vorbis_info_psy_global psy_g_param;
};
struct vorbis_look_psy_global { float ampmax; int channels; vorbis_info_psy_global* gi; int coupling_pointlimit[2][3]; };
static vorbis_look_psy_global* global_look(vorbis_info* vi)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    vorbis_info_psy_global* gi = &ci->psy_g_param;
    vorbis_look_psy_global* look = (vorbis_look_psy_global*)std::calloc(1, sizeof(*look));
    look->channels = vi->channels;
    look->ampmax = -9999.f;
    look->gi = gi;
    return look;
}
int main()
{
    codec_setup_info* ci = (codec_setup_info*)std::calloc(1, sizeof(codec_setup_info));
    vorbis_info vi = { 0, 6, 48000, 0, 0, 0, 0, ci };
    vorbis_look_psy_global* look = global_look(&vi);
    int ok = look && look->channels == 6 && look->ampmax == -9999.f && look->gi == &ci->psy_g_param &&
             look->coupling_pointlimit[0][0] == 0 && look->coupling_pointlimit[1][2] == 0;
    std::free(look); std::free(ci);
    if (!ok) { std::printf("00c91820_TEST FAIL\n"); return 1; }
    std::printf("00c91820_TEST PASS\n");
    return 0;
}