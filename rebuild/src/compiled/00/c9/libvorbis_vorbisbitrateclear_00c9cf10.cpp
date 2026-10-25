// libvorbis::vorbis_bitrate_clear(bitrate_manager_state* bm) -- free the bitrate manager's queues, accumulators, min/max stacks and queued packet buffers, then wipe it
// Leaf: vorbis_bitrate_clear
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9CF10, 185 bytes. libvorbis 1.0.1 lib/bitrate.c, compiled as C with
//   /Gr: `__fastcall`, bm in ecx, plain `ret`.
// Callees: free (MSVCR71 import [0x01440154], hoisted into ebp) through _ogg_free; libogg oggpack_writeclear
//   0x00C91320 (`__fastcall`, the 20-byte oggpack_buffer in ecx; landed as "FreeBufferHeader").
// Layout: libvorbis 1.0.1 bitrate.h bitrate_manager_state (0x68 bytes: queue_binned +0x00, queue_actual +0x04,
//   queue_size +0x08, avg_binacc +0x14, minmax_binstack/posstack/limitstack +0x30/+0x34/+0x38, the double
//   avgfloat +0x58, packetbuffers +0x60, packets +0x64).
// Shape: the shipped source; the per-buffer loop steps a byte offset by 0x14 (`add ebx,14h`) and re-reads
//   queue_size; memset(bm,0,0x68) is `rep stosd` x26.
extern "C" __declspec(dllimport) void __cdecl free(void* block);   // [0x01440154]
extern "C" void* __cdecl memset(void* dst, int c, unsigned int n);
#pragma intrinsic(memset)
#define _ogg_free free

typedef unsigned int ogg_uint32_t;
typedef __int64 ogg_int64_t;

typedef struct oggpack_buffer {
    long           endbyte;
    int            endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long           storage;
} oggpack_buffer;

typedef struct ogg_packet {
    unsigned char* packet;
    long           bytes;
    long           b_o_s;
    long           e_o_s;
    ogg_int64_t    granulepos;
    ogg_int64_t    packetno;
} ogg_packet;

typedef struct bitrate_manager_state {
    ogg_uint32_t* queue_binned;
    ogg_uint32_t* queue_actual;
    int           queue_size;
    int           queue_head;
    int           queue_bins;
    long*         avg_binacc;
    int           avg_center;
    int           avg_tail;
    ogg_uint32_t  avg_centeracc;
    ogg_uint32_t  avg_sampleacc;
    ogg_uint32_t  avg_sampledesired;
    ogg_uint32_t  avg_centerdesired;
    int*          minmax_binstack;
    int*          minmax_posstack;
    int*          minmax_limitstack;
    long          minmax_stackptr;
    long          minmax_acctotal;
    int           minmax_tail;
    ogg_uint32_t  minmax_sampleacc;
    ogg_uint32_t  minmax_sampledesired;
    int           next_to_flush;
    int           last_to_flush;
    double        avgfloat;
    oggpack_buffer* packetbuffers;
    ogg_packet*     packets;
} bitrate_manager_state;

extern "C" void __fastcall oggpack_writeclear(oggpack_buffer* b);   // 0x00C91320

extern "C" void __fastcall vorbis_bitrate_clear(bitrate_manager_state* bm)
{
    int i;
    if (bm) {
        if (bm->queue_binned) _ogg_free(bm->queue_binned);
        if (bm->queue_actual) _ogg_free(bm->queue_actual);
        if (bm->avg_binacc) _ogg_free(bm->avg_binacc);
        if (bm->minmax_binstack) _ogg_free(bm->minmax_binstack);
        if (bm->minmax_posstack) _ogg_free(bm->minmax_posstack);
        if (bm->minmax_limitstack) _ogg_free(bm->minmax_limitstack);

        if (bm->packetbuffers) {
            if (bm->queue_size == 0) {
                oggpack_writeclear(bm->packetbuffers);
            } else {
                for (i = 0; i < bm->queue_size; i++)
                    oggpack_writeclear(bm->packetbuffers + i);
            }
            _ogg_free(bm->packetbuffers);
        }
        if (bm->packets) _ogg_free(bm->packets);

        memset(bm, 0, sizeof(*bm));
    }
}