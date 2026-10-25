// libvorbis::limit_sum(bitrate_manager_state* bm, int limit) -- bytes the min/max window would hold if every queued packet were limited to bin `limit`
// Grade: DIFFER(170v178) -- LTCG register argument: retail takes bm in EBX (limit on the stack, `ret 4`); ours bm/limit in ecx/edx
// Retail: Fable.exe @ 0x00C9CC60, 178 bytes. libvorbis 1.0.1 lib/bitrate.c (static, LIMITBYTES macro). Grades: 1.
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: limit_sum
typedef unsigned int ogg_uint32_t;
typedef struct bitrate_manager_state {
    ogg_uint32_t* queue_binned; ogg_uint32_t* queue_actual; int queue_size; int queue_head; int queue_bins;
    long* avg_binacc; int avg_center; int avg_tail; ogg_uint32_t avg_centeracc; ogg_uint32_t avg_sampleacc;
    ogg_uint32_t avg_sampledesired; ogg_uint32_t avg_centerdesired;
    int* minmax_binstack; int* minmax_posstack; int* minmax_limitstack; long minmax_stackptr;
    long minmax_acctotal; int minmax_tail; ogg_uint32_t minmax_sampleacc; ogg_uint32_t minmax_sampledesired;
    int next_to_flush; int last_to_flush; double avgfloat; void* packetbuffers; void* packets;
} bitrate_manager_state;
#define LIMITBYTES(pos, bin) (bm->minmax_binstack[(pos) * bins * 2 + ((bin) + bins)])
extern "C" long __fastcall limit_sum(bitrate_manager_state* bm, int limit)
{
    int i = bm->minmax_stackptr;
    long acc = bm->minmax_acctotal;
    long bins = bm->queue_bins;

    acc -= LIMITBYTES(0, 0);
    acc += LIMITBYTES(0, limit);

    while (i-- > 0) {
        if (bm->minmax_limitstack[i] <= limit) break;
        acc -= LIMITBYTES(i, bm->minmax_limitstack[i]);
        acc += LIMITBYTES(i, limit);
    }
    return acc;
}
