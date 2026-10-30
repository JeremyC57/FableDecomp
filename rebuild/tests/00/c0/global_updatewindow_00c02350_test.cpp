// Behaviour test for 0x00C02350 zlib 1.2.1 updatewindow: window allocation on first use, full-window copy, and the
// circular split copy with write/whave bookkeeping.
#include <cstdio>
#include <cstdlib>
#include <cstring>

typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;
typedef void* voidpf;
typedef voidpf (*alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void (*free_func)(voidpf opaque, voidpf address);
struct z_stream {
    Bytef* next_in; uInt avail_in; uLong total_in; Bytef* next_out; uInt avail_out; uLong total_out; char* msg;
    void* state; alloc_func zalloc; free_func zfree; voidpf opaque; int data_type; uLong adler; uLong reserved;
};
typedef z_stream* z_streamp;
struct inflate_state {
    int mode, last, wrap, havedict, flags; unsigned long check, total;
    unsigned wbits, wsize, whave, write; unsigned char* window;
};
#define ZALLOC(strm, items, size) (*((strm)->zalloc))((strm)->opaque, (items), (size))
static int g_allocs = 0;
static voidpf test_alloc(voidpf, uInt items, uInt size) { ++g_allocs; return std::malloc(items * size); }

static int updatewindow(z_streamp strm, unsigned out)
{
    struct inflate_state* state;
    unsigned copy, dist;
    state = (struct inflate_state*)strm->state;
    if (state->window == 0) {
        state->window = (unsigned char*)ZALLOC(strm, 1U << state->wbits, sizeof(unsigned char));
        if (state->window == 0) return 1;
    }
    if (state->wsize == 0) {
        state->wsize = 1U << state->wbits;
        state->write = 0;
        state->whave = 0;
    }
    copy = out - strm->avail_out;
    if (copy >= state->wsize) {
        std::memcpy(state->window, strm->next_out - state->wsize, state->wsize);
        state->write = 0;
        state->whave = state->wsize;
    }
    else {
        dist = state->wsize - state->write;
        if (dist > copy) dist = copy;
        std::memcpy(state->window + state->write, strm->next_out - copy, dist);
        copy -= dist;
        if (copy) {
            std::memcpy(state->window, strm->next_out - copy, copy);
            state->write = copy;
            state->whave = state->wsize;
        }
        else {
            state->write += dist;
            if (state->write == state->wsize) state->write = 0;
            if (state->whave < state->wsize) state->whave += dist;
        }
    }
    return 0;
}

int main()
{
    unsigned char outbuf[64];
    for (int i = 0; i < 64; ++i) outbuf[i] = (unsigned char)i;
    inflate_state st; std::memset(&st, 0, sizeof(st)); st.wbits = 3;          // 8-byte window
    z_stream s; std::memset(&s, 0, sizeof(s));
    s.state = &st; s.zalloc = test_alloc;
    // 5 bytes produced: no wrap
    s.next_out = outbuf + 5; s.avail_out = 10;
    int bad = updatewindow(&s, 15) != 0 || g_allocs != 1 || st.wsize != 8 || st.write != 5 || st.whave != 5
              || std::memcmp(st.window, outbuf, 5) != 0;
    // 6 more bytes: 3 fill to the end, 3 wrap to the start
    s.next_out = outbuf + 11; s.avail_out = 4;
    bad |= updatewindow(&s, 10) != 0 || g_allocs != 1 || st.write != 3 || st.whave != 8
           || st.window[5] != 5 || st.window[7] != 7 || st.window[0] != 8 || st.window[2] != 10;
    // 20 bytes: whole window replaced by the last 8
    s.next_out = outbuf + 40; s.avail_out = 0;
    bad |= updatewindow(&s, 20) != 0 || st.write != 0 || st.whave != 8 || st.window[0] != 32 || st.window[7] != 39;
    // exactly filling the remainder resets write to 0
    st.write = 6; st.whave = 6;
    s.next_out = outbuf + 2; s.avail_out = 0;
    bad |= updatewindow(&s, 2) != 0 || st.write != 0 || st.whave != 8 || st.window[6] != 0 || st.window[7] != 1;
    std::free(st.window);
    if (bad) { std::printf("00c02350_TEST FAIL\n"); return 1; }
    std::printf("00c02350_TEST PASS\n");
    return 0;
}