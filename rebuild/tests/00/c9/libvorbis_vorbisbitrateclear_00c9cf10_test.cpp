// Behaviour test for 0x00C9CF10 libvorbis vorbis_bitrate_clear (copy of the model + logic; counting frees, oggpack_writeclear stand-in).
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef unsigned int u32;
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
struct bitrate_manager_state {
    u32* queue_binned; u32* queue_actual; int queue_size; int queue_head; int queue_bins; long* avg_binacc;
    int avg_center, avg_tail; u32 avg_centeracc, avg_sampleacc, avg_sampledesired, avg_centerdesired;
    int* minmax_binstack; int* minmax_posstack; int* minmax_limitstack; long minmax_stackptr; long minmax_acctotal;
    int minmax_tail; u32 minmax_sampleacc, minmax_sampledesired; int next_to_flush, last_to_flush; double avgfloat;
    oggpack_buffer* packetbuffers; void* packets;
};
static int g_frees, g_clears;
static void count_free(void* p) { g_frees++; std::free(p); }
static void writeclear_t(oggpack_buffer* b) { g_clears++; if (b->buffer) std::free(b->buffer); std::memset(b, 0, sizeof(*b)); }
static void bitrate_clear(bitrate_manager_state* bm)
{
    int i;
    if (bm) {
        if (bm->queue_binned) count_free(bm->queue_binned);
        if (bm->queue_actual) count_free(bm->queue_actual);
        if (bm->avg_binacc) count_free(bm->avg_binacc);
        if (bm->minmax_binstack) count_free(bm->minmax_binstack);
        if (bm->minmax_posstack) count_free(bm->minmax_posstack);
        if (bm->minmax_limitstack) count_free(bm->minmax_limitstack);
        if (bm->packetbuffers) {
            if (bm->queue_size == 0) writeclear_t(bm->packetbuffers);
            else for (i = 0; i < bm->queue_size; i++) writeclear_t(bm->packetbuffers + i);
            count_free(bm->packetbuffers);
        }
        if (bm->packets) count_free(bm->packets);
        std::memset(bm, 0, sizeof(*bm));
    }
}
int main()
{
    int ok = 1;
    bitrate_manager_state bm; std::memset(&bm, 0, sizeof(bm));
    bm.queue_size = 3; bm.queue_binned = (u32*)std::malloc(4); bm.avg_binacc = (long*)std::malloc(4);
    bm.minmax_limitstack = (int*)std::malloc(4);
    bm.packetbuffers = (oggpack_buffer*)std::calloc(3, sizeof(oggpack_buffer));
    bm.packets = std::malloc(4); bm.avgfloat = 2.5;
    bitrate_clear(&bm);
    if (g_frees != 5 || g_clears != 3 || bm.queue_size || bm.avgfloat != 0.0 || bm.packetbuffers) ok = 0;
    g_frees = g_clears = 0;
    bm.packetbuffers = (oggpack_buffer*)std::calloc(1, sizeof(oggpack_buffer));   /* queue_size 0: one buffer */
    bitrate_clear(&bm);
    if (g_frees != 1 || g_clears != 1) ok = 0;
    bitrate_clear(0);
    if (!ok) { std::printf("00c9cf10_TEST FAIL\n"); return 1; }
    std::printf("00c9cf10_TEST PASS\n");
    return 0;
}