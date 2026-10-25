// Behaviour test for 0x00C8F0D0 _vorbis_block_alloc (libvorbis block.c): copy of the logic.
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
int main()
{
    vorbis_block vb; memset(&vb, 0, sizeof(vb));
    char* a = (char*)_vorbis_block_alloc(&vb, 5);                  // first store: exactly 8 bytes
    int ok = vb.localalloc == 8 && vb.localtop == 8 && a == (char*)vb.localstore && vb.reap == 0 && vb.totaluse == 0;
    void* first = vb.localstore;
    char* b = (char*)_vorbis_block_alloc(&vb, 20);                 // does not fit: retire the 8-byte store
    ok &= vb.reap != 0 && vb.reap->ptr == first && vb.reap->next == 0 && vb.totaluse == 8;
    ok &= vb.localalloc == 24 && vb.localtop == 24 && b == (char*)vb.localstore;
    char* c = (char*)_vorbis_block_alloc(&vb, 0);                  // zero bytes fit at the top
    ok &= c == (char*)vb.localstore + 24 && vb.localtop == 24;
    free(vb.reap->ptr); free(vb.reap); free(vb.localstore);
    if (!ok) { std::printf("00c8f0d0_TEST FAIL\n"); return 1; }
    std::printf("00c8f0d0_TEST PASS\n");
    return 0;
}