// Behaviour test for libvorbis info.c _vorbis_unpack_info (0x00C8E120): read and validate the identification header body.
#include <cstdio>
struct rd { const long* v; int n; };
static long oggpack_read(rd* b, int) { return b->v[b->n++]; }
struct ci_t { long blocksizes[2]; };
struct vi_t { int version, channels; long rate, bitrate_upper, bitrate_nominal, bitrate_lower; ci_t* codec_setup; int cleared; };
static void vorbis_info_clear(vi_t* vi) { vi->cleared = 1; }
static int _vorbis_unpack_info(vi_t* vi, rd* opb)
{
    ci_t* ci = vi->codec_setup;
    if (!ci) return -129;
    vi->version = oggpack_read(opb, 32);
    if (vi->version != 0) return -134;
    vi->channels = oggpack_read(opb, 8); vi->rate = oggpack_read(opb, 32);
    vi->bitrate_upper = oggpack_read(opb, 32); vi->bitrate_nominal = oggpack_read(opb, 32); vi->bitrate_lower = oggpack_read(opb, 32);
    ci->blocksizes[0] = 1 << oggpack_read(opb, 4); ci->blocksizes[1] = 1 << oggpack_read(opb, 4);
    if (vi->rate < 1 || vi->channels < 1 || ci->blocksizes[0] < 8 || ci->blocksizes[1] < ci->blocksizes[0]) goto err_out;
    if (oggpack_read(opb, 1) != 1) goto err_out;
    return 0;
err_out:
    vorbis_info_clear(vi); return -133;
}
int main()
{
    static const long good[] = { 0, 2, 44100, -1, 128000, -1, 8, 11, 1 }, bad[] = { 0, 2, 44100, 0, 0, 0, 11, 8, 1 };
    ci_t ci; vi_t vi; vi.codec_setup = &ci; vi.cleared = 0;
    rd r = { good, 0 };
    if (_vorbis_unpack_info(&vi, &r) != 0 || vi.channels != 2 || ci.blocksizes[0] != 256 || ci.blocksizes[1] != 2048) { std::printf("00c8e120_TEST FAIL\n"); return 1; }
    rd r2 = { bad, 0 };
    if (_vorbis_unpack_info(&vi, &r2) != -133 || !vi.cleared) { std::printf("00c8e120_TEST FAIL\n"); return 1; }
    vi.codec_setup = 0;
    if (_vorbis_unpack_info(&vi, &r2) != -129) { std::printf("00c8e120_TEST FAIL\n"); return 1; }
    std::printf("00c8e120_TEST PASS\n"); return 0;
}