// Behaviour test for 0x00C8F880 vorbis_analysis_buffer (libvorbis block.c): copy of the logic.
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
float** vorbis_analysis_buffer(vorbis_dsp_state* v, int vals)
{
    int i;
    vorbis_info* vi = v->vi;
    private_state* b = (private_state*)v->backend_state;
    if (b->header) free(b->header); b->header = 0;
    if (b->header1) free(b->header1); b->header1 = 0;
    if (b->header2) free(b->header2); b->header2 = 0;
    if (v->pcm_current + vals >= v->pcm_storage) {
        v->pcm_storage = v->pcm_current + vals * 2;
        for (i = 0; i < vi->channels; i++) {
            v->pcm[i] = (float*)realloc(v->pcm[i], v->pcm_storage * sizeof(*v->pcm[i]));
        }
    }
    for (i = 0; i < vi->channels; i++)
        v->pcmret[i] = v->pcm[i] + v->pcm_current;
    return (v->pcmret);
}
int main()
{
    vorbis_info vi; memset(&vi, 0, sizeof(vi)); vi.channels = 2;
    private_state ps; memset(&ps, 0, sizeof(ps)); ps.header = (unsigned char*)malloc(4); ps.header2 = (unsigned char*)malloc(4);
    float* pcm[2]; float* ret[2]; pcm[0] = (float*)malloc(16 * sizeof(float)); pcm[1] = (float*)malloc(16 * sizeof(float));
    vorbis_dsp_state v; memset(&v, 0, sizeof(v)); v.vi = &vi; v.backend_state = &ps; v.pcm = pcm; v.pcmret = ret;
    v.pcm_storage = 16; v.pcm_current = 10;
    pcm[0][9] = 1.5f;
    float** r = vorbis_analysis_buffer(&v, 4);                     // 14 < 16: no growth
    int ok = r == ret && ret[0] == pcm[0] + 10 && ret[1] == pcm[1] + 10 && ps.header == 0 && ps.header1 == 0 && ps.header2 == 0;
    ok &= v.pcm_storage == 16;
    r = vorbis_analysis_buffer(&v, 6);                             // 16 >= 16: grow to 10 + 12
    ok &= v.pcm_storage == 22 && ret[0] == pcm[0] + 10 && ret[1] == pcm[1] + 10 && pcm[0][9] == 1.5f;
    free(pcm[0]); free(pcm[1]);
    if (!ok) { std::printf("00c8f880_TEST FAIL\n"); return 1; }
    std::printf("00c8f880_TEST PASS\n");
    return 0;
}