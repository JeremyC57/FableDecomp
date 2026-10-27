#include <cstdio>
// vorbis_synthesis_halfrate: refuse half rate with 64-sample short blocks; else set halfrate_flag = flag?1:0.
struct codec_setup_info { int blocksizes[2]; int halfrate_flag; };
struct vorbis_info { int channels; codec_setup_info* codec_setup; };
static int vorbis_synthesis_halfrate(vorbis_info* vi, int flag)
{
    codec_setup_info* ci = vi->codec_setup;
    if (ci->blocksizes[0] <= 64 && flag) return -1;
    ci->halfrate_flag = (flag ? 1 : 0);
    return 0;
}
int main()
{
    codec_setup_info ci = { { 256, 2048 }, 0 };
    vorbis_info vi = { 2, &ci };
    if (vorbis_synthesis_halfrate(&vi, 5) != 0 || ci.halfrate_flag != 1) { std::printf("00c8f030_TEST FAIL\n"); return 1; }
    if (vorbis_synthesis_halfrate(&vi, 0) != 0 || ci.halfrate_flag != 0) { std::printf("00c8f030_TEST FAIL\n"); return 1; }
    ci.blocksizes[0] = 64;
    if (vorbis_synthesis_halfrate(&vi, 1) != -1 || ci.halfrate_flag != 0) { std::printf("00c8f030_TEST FAIL\n"); return 1; }
    ci.halfrate_flag = 1;
    if (vorbis_synthesis_halfrate(&vi, 0) != 0 || ci.halfrate_flag != 0) { std::printf("00c8f030_TEST FAIL\n"); return 1; }
    std::printf("00c8f030_TEST PASS\n");
    return 0;
}