// Behaviour test for 0x00C8FEA0 vorbis_synthesis_restart (libvorbis block.c): copy of the logic.
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
int vorbis_synthesis_restart(vorbis_dsp_state* v)
{
    vorbis_info* vi = v->vi;
    codec_setup_info* ci;
    int hs;
    if (!v->backend_state) return -1;
    if (!vi) return -1;
    ci = (codec_setup_info*)vi->codec_setup;
    if (!ci) return -1;
    hs = ci->halfrate_flag;
    v->centerW = ci->blocksizes[1] >> (hs + 1);
    v->pcm_current = v->centerW >> hs;
    v->pcm_returned = -1;
    v->granulepos = -1;
    v->sequence = -1;
    v->eofflag = 0;
    ((private_state*)(v->backend_state))->sample_count = -1;
    return (0);
}
int main()
{
    codec_setup_info ci; memset(&ci, 0, sizeof(ci)); ci.blocksizes[0] = 256; ci.blocksizes[1] = 2048;
    vorbis_info vi; memset(&vi, 0, sizeof(vi)); vi.codec_setup = &ci;
    private_state ps; memset(&ps, 0, sizeof(ps));
    vorbis_dsp_state v; memset(&v, 0, sizeof(v)); v.vi = &vi; v.backend_state = &ps; v.eofflag = 1;
    int ok = vorbis_synthesis_restart(&v) == 0 && v.centerW == 1024 && v.pcm_current == 1024;
    ok &= v.pcm_returned == -1 && v.granulepos == -1 && v.sequence == -1 && v.eofflag == 0 && ps.sample_count == -1;
    ci.halfrate_flag = 1; ok &= vorbis_synthesis_restart(&v) == 0 && v.centerW == 512 && v.pcm_current == 256;
    vi.codec_setup = 0; ok &= vorbis_synthesis_restart(&v) == -1;
    v.vi = 0; ok &= vorbis_synthesis_restart(&v) == -1;
    v.vi = &vi; v.backend_state = 0; ok &= vorbis_synthesis_restart(&v) == -1;
    if (!ok) { std::printf("00c8fea0_TEST FAIL\n"); return 1; }
    std::printf("00c8fea0_TEST PASS\n");
    return 0;
}