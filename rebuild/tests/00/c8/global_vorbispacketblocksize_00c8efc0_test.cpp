// Behaviour test for 0x00C8EFC0 vorbis_packet_blocksize (libvorbis synthesis.c): copy of the logic with stand-in
// bit-reader callees (they also record the requested mode-bit count).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 ogg_int64_t;
#else
typedef long long ogg_int64_t;
#endif
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct ogg_packet { unsigned char* packet; long bytes; long b_o_s; long e_o_s; ogg_int64_t granulepos; ogg_int64_t packetno; };
struct vorbis_info { int version; int channels; long rate; long bu, bn, bl, bw; void* codec_setup; };
struct vorbis_info_mode { int blockflag; int windowtype; int transformtype; int mapping; };
struct codec_setup_info { long blocksizes[2]; int modes; int maps, floors, residues, books, psys; vorbis_info_mode* mode_param[64]; int halfrate_flag; };
struct vorbis_dsp_state {
    int analysisp; vorbis_info* vi; float** pcm; float** pcmret; int pcm_storage; int pcm_current; int pcm_returned;
    int preextrapolate; int eofflag; long lW; long W; long nW; long centerW; ogg_int64_t granulepos; ogg_int64_t sequence;
    ogg_int64_t glue_bits, time_bits, floor_bits, res_bits; void* backend_state;
};
struct private_state { void* ve; int modebits; void* psy_g_look; unsigned char* header; unsigned char* header1; unsigned char* header2; ogg_int64_t sample_count; };
struct alloc_chain { void* ptr; alloc_chain* next; };
struct vorbis_block {
    float** pcm; oggpack_buffer opb; long lW; long W; long nW; int pcmend; int mode; int eofflag;
    ogg_int64_t granulepos; ogg_int64_t sequence; vorbis_dsp_state* vd; void* localstore; long localtop; long localalloc;
    long totaluse; alloc_chain* reap; long glue_bits, time_bits, floor_bits, res_bits; void* internal;
};
static long g_bits[4]; static int g_nbits = 0, g_pos = 0, g_lastbits = 0;
void oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes) { b->buffer = b->ptr = buf; b->storage = bytes; g_pos = 0; }
long oggpack_read(oggpack_buffer* b, int bits) { (void)b; g_lastbits = bits; return g_pos < g_nbits ? g_bits[g_pos++] : -1; }
#define OV_ENOTAUDIO  -135
#define OV_EBADPACKET -136
long vorbis_packet_blocksize(vorbis_info* vi, ogg_packet* op)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    oggpack_buffer    opb;
    int               mode;
    oggpack_readinit(&opb, op->packet, op->bytes);
    if (oggpack_read(&opb, 1) != 0) {
        return (OV_ENOTAUDIO);
    }
    {
        int modebits = 0;
        int v = ci->modes;
        while (v > 1) {
            modebits++;
            v >>= 1;
        }
        mode = oggpack_read(&opb, modebits);
    }
    if (mode == -1) return (OV_EBADPACKET);
    return (ci->blocksizes[ci->mode_param[mode]->blockflag]);
}
int main()
{
    vorbis_info_mode m[5]; int i; for (i = 0; i < 5; i++) m[i].blockflag = i & 1;
    codec_setup_info ci; memset(&ci, 0, sizeof(ci)); ci.blocksizes[0] = 256; ci.blocksizes[1] = 2048; ci.modes = 5;
    for (i = 0; i < 5; i++) ci.mode_param[i] = &m[i];
    vorbis_info vi; memset(&vi, 0, sizeof(vi)); vi.codec_setup = &ci;
    unsigned char pkt[2] = { 0, 0 };
    ogg_packet op; memset(&op, 0, sizeof(op)); op.packet = pkt; op.bytes = 2;
    g_bits[0] = 0; g_bits[1] = 3; g_nbits = 2;
    int ok = vorbis_packet_blocksize(&vi, &op) == 2048 && g_lastbits == 2;      // 5 modes -> 2 mode bits
    g_bits[1] = 4; ok &= vorbis_packet_blocksize(&vi, &op) == 256;
    ci.modes = 1; g_bits[1] = 0; ok &= vorbis_packet_blocksize(&vi, &op) == 256 && g_lastbits == 0;
    g_bits[0] = 1; ok &= vorbis_packet_blocksize(&vi, &op) == -135;
    g_bits[0] = 0; g_nbits = 1; ok &= vorbis_packet_blocksize(&vi, &op) == -136;
    if (!ok) { std::printf("00c8efc0_TEST FAIL\n"); return 1; }
    std::printf("00c8efc0_TEST PASS\n");
    return 0;
}