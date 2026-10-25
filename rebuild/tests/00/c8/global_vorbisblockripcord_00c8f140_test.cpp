// Behaviour test for 0x00C8F140 _vorbis_block_ripcord (libvorbis block.c): copy of the logic over _vorbis_block_alloc.
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
#define WORD_ALIGN 8
void* _vorbis_block_alloc(vorbis_block* vb, long bytes)
{
    bytes = (bytes + (WORD_ALIGN - 1)) & ~(WORD_ALIGN - 1);
    if (bytes + vb->localtop > vb->localalloc) {
        if (vb->localstore) {
            struct alloc_chain* link = (alloc_chain*)malloc(sizeof(*link));
            vb->totaluse += vb->localtop;
            link->next = vb->reap;
            link->ptr = vb->localstore;
            vb->reap = link;
        }
        vb->localalloc = bytes;
        vb->localstore = malloc(vb->localalloc);
        vb->localtop = 0;
    }
    {
        void* ret = (void*)(((char*)vb->localstore) + vb->localtop);
        vb->localtop += bytes;
        return ret;
    }
}
void _vorbis_block_ripcord(vorbis_block* vb)
{
    struct alloc_chain* reap = vb->reap;
    while (reap) {
        struct alloc_chain* next = reap->next;
        free(reap->ptr);
        memset(reap, 0, sizeof(*reap));
        free(reap);
        reap = next;
    }
    if (vb->totaluse) {
        vb->localstore = realloc(vb->localstore, vb->totaluse + vb->localalloc);
        vb->localalloc += vb->totaluse;
        vb->totaluse = 0;
    }
    vb->localtop = 0;
    vb->reap = 0;
}
int main()
{
    vorbis_block vb; memset(&vb, 0, sizeof(vb));
    _vorbis_block_alloc(&vb, 16); _vorbis_block_alloc(&vb, 40); _vorbis_block_alloc(&vb, 100);   // two retired stores
    int ok = vb.totaluse == 16 + 40 && vb.reap != 0 && vb.reap->next != 0;
    _vorbis_block_ripcord(&vb);
    ok &= vb.reap == 0 && vb.totaluse == 0 && vb.localtop == 0 && vb.localalloc == 104 + 56 && vb.localstore != 0;
    _vorbis_block_ripcord(&vb);                                    // nothing to reap or consolidate
    ok &= vb.localalloc == 160 && vb.localtop == 0;
    char* p = (char*)_vorbis_block_alloc(&vb, 150);                 // fits in the consolidated store
    ok &= p == (char*)vb.localstore && vb.reap == 0;
    free(vb.localstore);
    if (!ok) { std::printf("00c8f140_TEST FAIL\n"); return 1; }
    std::printf("00c8f140_TEST PASS\n");
    return 0;
}