// Behaviour test for libvorbis info.c _vorbis_pack_info (0x00C8E6E0): write the identification header fields.
#include <cstdio>
struct wr { unsigned long v[32]; int bits[32]; int n; };
static void oggpack_write(wr* b, unsigned long value, int bits) { b->v[b->n] = value; b->bits[b->n++] = bits; }
static void _v_writestring(wr* o, const char* s, int bytes) { while (bytes--) oggpack_write(o, *s++, 8); }
static int ilog2(unsigned int v) { int ret = 0; if (v) --v; while (v) { ret++; v >>= 1; } return ret; }
struct ci_t { long blocksizes[2]; };
struct vi_t { int channels; long rate, bitrate_upper, bitrate_nominal, bitrate_lower; ci_t* codec_setup; };
static int _vorbis_pack_info(wr* opb, vi_t* vi)
{
    ci_t* ci = vi->codec_setup;
    if (!ci) return -129;
    oggpack_write(opb, 0x01, 8); _v_writestring(opb, "vorbis", 6);
    oggpack_write(opb, 0x00, 32); oggpack_write(opb, vi->channels, 8); oggpack_write(opb, vi->rate, 32);
    oggpack_write(opb, vi->bitrate_upper, 32); oggpack_write(opb, vi->bitrate_nominal, 32); oggpack_write(opb, vi->bitrate_lower, 32);
    oggpack_write(opb, ilog2(ci->blocksizes[0]), 4); oggpack_write(opb, ilog2(ci->blocksizes[1]), 4); oggpack_write(opb, 1, 1);
    return 0;
}
int main()
{
    ci_t ci = { { 256, 2048 } }; vi_t vi = { 2, 44100, 0, 128000, 0, &ci }; wr w; w.n = 0;
    if (_vorbis_pack_info(&w, &vi) != 0 || w.n != 16 || w.v[1] != 'v' || w.v[8] != 2 || w.v[13] != 8 || w.v[14] != 11 || w.bits[15] != 1)
    { std::printf("00c8e6e0_TEST FAIL\n"); return 1; }
    vi.codec_setup = 0;
    if (_vorbis_pack_info(&w, &vi) != -129) { std::printf("00c8e6e0_TEST FAIL\n"); return 1; }
    std::printf("00c8e6e0_TEST PASS\n"); return 0;
}