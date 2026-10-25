// Behaviour test for 0x00C8EEF0 vorbis_synthesis_trackonly (libvorbis synthesis.c): copy of the logic with stand-in
// bit-reader callees that return a scripted sequence of fields.
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
static long g_bits[8]; static int g_nbits = 0, g_pos = 0, g_ripcords = 0;
static unsigned char* g_buf = 0; static int g_bytes = 0;
void _vorbis_block_ripcord(vorbis_block* vb) { (void)vb; g_ripcords++; }
void oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes) { b->buffer = b->ptr = buf; b->storage = bytes; g_buf = buf; g_bytes = bytes; g_pos = 0; }
long oggpack_read(oggpack_buffer* b, int bits) { (void)b; (void)bits; return g_pos < g_nbits ? g_bits[g_pos++] : -1; }
#define OV_ENOTAUDIO  -135
#define OV_EBADPACKET -136
int vorbis_synthesis_trackonly(vorbis_block* vb, ogg_packet* op)
{
    vorbis_dsp_state* vd = vb->vd;
    private_state*    b = (private_state*)vd->backend_state;
    vorbis_info*      vi = vd->vi;
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    oggpack_buffer*   opb = &vb->opb;
    int               mode;

    _vorbis_block_ripcord(vb);
    oggpack_readinit(opb, op->packet, op->bytes);
    if (oggpack_read(opb, 1) != 0) {
        return (OV_ENOTAUDIO);
    }
    mode = oggpack_read(opb, b->modebits);
    if (mode == -1) return (OV_EBADPACKET);

    vb->mode = mode;
    vb->W = ci->mode_param[mode]->blockflag;
    if (vb->W) {
        vb->lW = oggpack_read(opb, 1);
        vb->nW = oggpack_read(opb, 1);
        if (vb->nW == -1) return (OV_EBADPACKET);
    } else {
        vb->lW = 0;
        vb->nW = 0;
    }
    vb->granulepos = op->granulepos;
    vb->sequence = op->packetno;
    vb->eofflag = op->e_o_s;
    vb->pcmend = 0;
    vb->pcm = 0;
    return (0);
}
int main()
{
    vorbis_info_mode m0, m1; m0.blockflag = 0; m1.blockflag = 1;
    codec_setup_info ci; memset(&ci, 0, sizeof(ci)); ci.mode_param[0] = &m0; ci.mode_param[1] = &m1;
    vorbis_info vi; memset(&vi, 0, sizeof(vi)); vi.codec_setup = &ci;
    private_state ps; memset(&ps, 0, sizeof(ps)); ps.modebits = 1;
    vorbis_dsp_state vd; memset(&vd, 0, sizeof(vd)); vd.vi = &vi; vd.backend_state = &ps;
    vorbis_block vb; memset(&vb, 0x33, sizeof(vb)); vb.vd = &vd;
    unsigned char pkt[4] = { 0, 1, 2, 3 };
    ogg_packet op; op.packet = pkt; op.bytes = 4; op.b_o_s = 0; op.e_o_s = 1; op.granulepos = 4096; op.packetno = 7;
    g_bits[0] = 0; g_bits[1] = 1; g_bits[2] = 1; g_bits[3] = 0; g_nbits = 4;           // audio, mode 1 (long), lW 1, nW 0
    int ok = vorbis_synthesis_trackonly(&vb, &op) == 0;
    ok &= g_ripcords == 1 && g_buf == pkt && g_bytes == 4 && vb.mode == 1 && vb.W == 1 && vb.lW == 1 && vb.nW == 0;
    ok &= vb.granulepos == 4096 && vb.sequence == 7 && vb.eofflag == 1 && vb.pcmend == 0 && vb.pcm == 0;
    g_bits[1] = 0; g_nbits = 2;                                                          // mode 0 (short): lW = nW = 0
    ok &= vorbis_synthesis_trackonly(&vb, &op) == 0 && vb.W == 0 && vb.lW == 0 && vb.nW == 0;
    g_bits[0] = 1; ok &= vorbis_synthesis_trackonly(&vb, &op) == -135;                   // not an audio packet
    g_bits[0] = 0; g_nbits = 1; ok &= vorbis_synthesis_trackonly(&vb, &op) == -136;      // mode read fails
    g_bits[1] = 1; g_bits[2] = 0; g_nbits = 3; ok &= vorbis_synthesis_trackonly(&vb, &op) == -136;   // nW read fails
    if (!ok) { std::printf("00c8eef0_TEST FAIL\n"); return 1; }
    std::printf("00c8eef0_TEST PASS\n");
    return 0;
}