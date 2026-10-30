// Behaviour test for zlib deflate.c flush_pending (0x00C00630): move min(pending, avail_out) bytes to next_out.
#include <cstdio>
#include <cstring>
typedef unsigned char Bytef; typedef unsigned int uInt; typedef unsigned long uLong;
struct internal_state { Bytef* pending_buf; Bytef* pending_out; int pending; };
struct z_stream { Bytef* next_out; uInt avail_out; uLong total_out; internal_state* state; };
static void flush_pending(z_stream* strm)
{
    unsigned len = strm->state->pending;
    if (len > strm->avail_out) len = strm->avail_out;
    if (len == 0) return;
    memcpy(strm->next_out, strm->state->pending_out, len);
    strm->next_out += len; strm->state->pending_out += len; strm->total_out += len;
    strm->avail_out -= len; strm->state->pending -= len;
    if (strm->state->pending == 0) strm->state->pending_out = strm->state->pending_buf;
}
int main()
{
    Bytef pend[6] = { 1, 2, 3, 4, 5, 6 }, out[8] = { 0 };
    internal_state st = { pend, pend, 6 }; z_stream z = { out, 4, 0, &st };
    flush_pending(&z);
    if (z.avail_out != 0 || z.total_out != 4 || st.pending != 2 || st.pending_out != pend + 4 || out[3] != 4) { std::printf("00c00630_TEST FAIL\n"); return 1; }
    z.avail_out = 4; flush_pending(&z);
    if (st.pending != 0 || st.pending_out != pend || out[5] != 6 || z.total_out != 6) { std::printf("00c00630_TEST FAIL\n"); return 1; }
    std::printf("00c00630_TEST PASS\n"); return 0;
}