// Test for _global::deflate_stored(deflate_state* s, int flush) -- zlib 1.2.1: input is cut into stored blocks of at most min(0xffff, pending_buf_size - 5) bytes; with no input and Z_NO_FLUSH it asks for more, at Z_FINISH it emits the last block and reports finish_done.
#include <cstdio>
enum block_state { need_more, block_done, finish_started, finish_done };
enum { Z_NO_FLUSH = 0, Z_FINISH = 4 };
struct State {
    unsigned long pending_buf_size; unsigned int lookahead, strstart, w_size; long block_start;
    unsigned int input;              // bytes the stand-in fill_window still has
    int blocks; unsigned long sizes[16]; int lastFlag; unsigned int avail_out;
};
static void fill_window(State* s) { unsigned int n = s->input > 40000 ? 40000 : s->input; s->lookahead += n; s->input -= n; }
static void flush_block(State* s, int eof)
{
    s->sizes[s->blocks++] = (unsigned long)(s->strstart - s->block_start);
    s->lastFlag = eof; s->block_start = (long)s->strstart;
}
#define MAX_DIST(s) ((s)->w_size - 262)
#define FLUSH_BLOCK(s, eof) { flush_block(s, eof); if ((s)->avail_out == 0) return (eof) ? finish_started : need_more; }
static block_state deflate_stored(State* s, int flush)
{
    unsigned long max_block_size = 0xffff;
    unsigned long max_start;
    if (max_block_size > s->pending_buf_size - 5)
        max_block_size = s->pending_buf_size - 5;
    for (;;) {
        if (s->lookahead <= 1) {
            fill_window(s);
            if (s->lookahead == 0 && flush == Z_NO_FLUSH) return need_more;
            if (s->lookahead == 0) break;
        }
        s->strstart += s->lookahead;
        s->lookahead = 0;
        max_start = s->block_start + max_block_size;
        if (s->strstart == 0 || (unsigned long)s->strstart >= max_start) {
            s->lookahead = (unsigned int)(s->strstart - max_start);
            s->strstart = (unsigned int)max_start;
            FLUSH_BLOCK(s, 0);
        }
        if (s->strstart - (unsigned int)s->block_start >= MAX_DIST(s)) {
            FLUSH_BLOCK(s, 0);
        }
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return flush == Z_FINISH ? finish_done : block_done;
}
int main()
{
    State s = { 0x4000 + 5, 0, 0, 0x100000, 0, 0x5000, 0, {0}, -1, 1 };
    int ok = 1;
    if (deflate_stored(&s, Z_FINISH) != finish_done) ok = 0;
    // 0x5000 bytes with 0x4000-byte blocks: one full block, then the 0x1000-byte last block
    if (s.blocks != 2 || s.sizes[0] != 0x4000 || s.sizes[1] != 0x1000 || s.lastFlag != 1) ok = 0;
    State e = { 0x10000, 0, 0, 0x100000, 0, 0, 0, {0}, -1, 1 };
    if (deflate_stored(&e, Z_NO_FLUSH) != need_more || e.blocks != 0) ok = 0;
    if (!ok) { std::printf("00c011a0_TEST FAIL\n"); return 1; }
    std::printf("00c011a0_TEST PASS\n");
    return 0;
}