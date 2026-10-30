// Behaviour test for 0x00C9CB40 BINBYTES (see src/00c9cb40.cpp): queue_binned[pos * queue_bins + bin].
#include <cstdio>
typedef unsigned int ogg_uint32_t;
struct bitrate_manager_state { ogg_uint32_t* queue_binned; ogg_uint32_t* queue_actual; int queue_size; int queue_head; int queue_bins; };
static long BINBYTES(bitrate_manager_state* bm, long pos, long bin) { int bins = bm->queue_bins; return (long)bm->queue_binned[pos * bins + bin]; }
int main()
{
    ogg_uint32_t q[12]; for (int i = 0; i < 12; i++) q[i] = 100 + i;
    bitrate_manager_state bm = { q, 0, 4, 0, 3 };
    if (BINBYTES(&bm, 0, 0) != 100 || BINBYTES(&bm, 2, 1) != 107 || BINBYTES(&bm, 3, 2) != 111) { std::printf("00c9cb40_TEST FAIL\n"); return 1; }
    std::printf("00c9cb40_TEST PASS\n"); return 0;
}