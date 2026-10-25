// Behaviour test for 0x00C8F1B0 vorbis_block_clear (libvorbis block.c): copy of the logic with stand-in
// oggpack_writeclear / _vorbis_block_ripcord that count their calls.
#include <cstdio>
#include <cstdlib>
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
static int g_writeclears = 0, g_ripcords = 0;
void oggpack_writeclear(oggpack_buffer* b) { (void)b; g_writeclears++; }
void _vorbis_block_ripcord(vorbis_block* vb) { (void)vb; g_ripcords++; }
int vorbis_block_clear(vorbis_block* vb)
{
    if (vb->vd)
        if (vb->vd->analysisp)
            oggpack_writeclear(&vb->opb);
    _vorbis_block_ripcord(vb);
    if (vb->localstore) free(vb->localstore);
    if (vb->internal) free(vb->internal);
    memset(vb, 0, sizeof(*vb));
    return (0);
}
int main()
{
    vorbis_dsp_state vd; memset(&vd, 0, sizeof(vd)); vd.analysisp = 1;
    vorbis_block vb; memset(&vb, 0, sizeof(vb)); vb.vd = &vd; vb.localstore = malloc(8); vb.internal = malloc(8); vb.mode = 3;
    int ok = vorbis_block_clear(&vb) == 0 && g_writeclears == 1 && g_ripcords == 1;
    ok &= vb.vd == 0 && vb.localstore == 0 && vb.internal == 0 && vb.mode == 0;
    vd.analysisp = 0; vb.vd = &vd; ok &= vorbis_block_clear(&vb) == 0 && g_writeclears == 1 && g_ripcords == 2;
    ok &= vorbis_block_clear(&vb) == 0 && g_writeclears == 1 && g_ripcords == 3;         // no vd
    if (!ok) { std::printf("00c8f1b0_TEST FAIL\n"); return 1; }
    std::printf("00c8f1b0_TEST PASS\n");
    return 0;
}