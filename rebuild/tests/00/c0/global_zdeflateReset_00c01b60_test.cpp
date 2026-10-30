// Behaviour test for zlib deflate.c deflateReset (0x00C01B60) with its static lm_init.
#include <cstdio>
#include <cstring>
typedef unsigned char Bytef; typedef unsigned int uInt; typedef unsigned long uLong; typedef unsigned long ulg; typedef unsigned short ush;
typedef void* (*alloc_func)(void*, uInt, uInt); typedef void (*free_func)(void*, void*);
struct internal_state;
struct z_stream { uLong total_in, total_out; char* msg; internal_state* state; alloc_func zalloc; free_func zfree; int data_type; uLong adler; };
struct internal_state { z_stream* strm; int status; Bytef* pending_buf; Bytef* pending_out; int pending; int wrap; int last_flush;
    uInt w_size; ulg window_size; ush* head; uInt ins_h; uInt hash_size; long block_start; uInt match_length; int match_available;
    uInt strstart; uInt lookahead; uInt prev_length; uInt max_chain_length; uInt max_lazy_match; int level; uInt good_match; int nice_match; };
typedef internal_state deflate_state;
struct config { ush good_length, max_lazy, nice_length, max_chain; };
static const config configuration_table[10] = { {0,0,0,0},{4,4,8,4},{4,5,16,8},{4,6,32,32},{4,4,16,16},{8,16,32,32},{8,16,128,128},{8,32,128,256},{32,128,258,1024},{32,258,258,4096} };
static int g_tr; static void _tr_init(deflate_state*) { ++g_tr; }
static uLong z_adler32(uLong, const Bytef*, uInt) { return 1; }
static uLong z_crc32(uLong, const Bytef*, uInt) { return 0; }
static void* za(void*, uInt, uInt) { return 0; } static void zf(void*, void*) { }
static void lm_init(deflate_state* s)
{
    s->window_size = (ulg)2L * s->w_size;
    s->head[s->hash_size - 1] = 0; memset(s->head, 0, (unsigned)(s->hash_size - 1) * sizeof(*s->head));
    s->max_lazy_match = configuration_table[s->level].max_lazy; s->good_match = configuration_table[s->level].good_length;
    s->nice_match = configuration_table[s->level].nice_length; s->max_chain_length = configuration_table[s->level].max_chain;
    s->strstart = 0; s->block_start = 0L; s->lookahead = 0; s->match_length = s->prev_length = 3 - 1; s->match_available = 0; s->ins_h = 0;
}
static int deflateReset(z_stream* strm)
{
    deflate_state* s;
    if (strm == 0 || strm->state == 0 || strm->zalloc == 0 || strm->zfree == 0) return -2;
    strm->total_in = strm->total_out = 0; strm->msg = 0; strm->data_type = 2;
    s = strm->state; s->pending = 0; s->pending_out = s->pending_buf;
    if (s->wrap < 0) s->wrap = -s->wrap;
    s->status = s->wrap ? 42 : 113;
    strm->adler = s->wrap == 2 ? z_crc32(0L, 0, 0) : z_adler32(0L, 0, 0);
    s->last_flush = 0;
    _tr_init(s); lm_init(s);
    return 0;
}
int main()
{
    static ush head[16]; static Bytef pb[4]; for (int i = 0; i < 16; i++) head[i] = 0x55;
    deflate_state s; memset(&s, 0, sizeof(s)); s.pending_buf = pb; s.pending = 9; s.wrap = -1; s.w_size = 0x8000; s.head = head; s.hash_size = 16; s.level = 6;
    z_stream z; memset(&z, 0, sizeof(z)); z.state = &s; z.zalloc = za; z.zfree = zf; z.total_in = 5;
    if (deflateReset(&z) != 0 || z.total_in != 0 || z.data_type != 2 || s.pending != 0 || s.wrap != 1 || s.status != 42 || z.adler != 1
        || g_tr != 1 || s.window_size != 0x10000 || head[15] != 0 || head[3] != 0 || s.max_chain_length != 128 || s.prev_length != 2)
    { std::printf("00c01b60_TEST FAIL\n"); return 1; }
    z.zalloc = 0; if (deflateReset(&z) != -2) { std::printf("00c01b60_TEST FAIL\n"); return 1; }
    std::printf("00c01b60_TEST PASS\n"); return 0;
}