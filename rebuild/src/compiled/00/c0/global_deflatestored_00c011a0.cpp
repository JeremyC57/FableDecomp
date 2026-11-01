// _global::deflate_stored(deflate_state* s, int flush) -- zlib 1.2.1 deflate.c deflate_stored: copy input to stored (uncompressed) blocks of at most min(0xffff, pending_buf_size-5) bytes, flushing whenever the block fills or the window would overflow
// Leaf: _deflate_stored
// Retail 0x00C011A0, 529 bytes, __cdecl. Identity: only reference is .rdata 0x012AE268 = zlib configuration_table[0].func;
//   body is zlib 1.2.1 deflate.c verbatim. Callees fill_window 0x00C01090, _tr_flush_block 0x00C057D0, flush_pending
//   0x00C00630 (a same-TU static: VC7.1 inlines two of its three FLUSH_BLOCK sites, as retail).
typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte           Bytef;
typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;
typedef ush            Pos;
typedef Pos            Posf;
typedef unsigned       IPos;
typedef void*          voidpf;
typedef voidpf (__cdecl *alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (__cdecl *free_func)(voidpf opaque, voidpf address);

struct internal_state;
typedef struct z_stream_s {
    Bytef*    next_in;      // +0x00
    uInt      avail_in;     // +0x04
    uLong     total_in;     // +0x08
    Bytef*    next_out;     // +0x0c
    uInt      avail_out;    // +0x10
    uLong     total_out;    // +0x14
    char*     msg;          // +0x18
    struct internal_state* state; // +0x1c
    alloc_func zalloc;      // +0x20
    free_func  zfree;       // +0x24
    voidpf     opaque;      // +0x28
    int        data_type;   // +0x2c
    uLong      adler;       // +0x30
    uLong      reserved;    // +0x34
} z_stream;                 // 0x38
typedef z_stream* z_streamp;
struct ct_data_s { union { ush freq; ush code; } fc; union { ush dad; ush len; } dl; };
typedef struct ct_data_s ct_data;
struct static_tree_desc_s;
struct tree_desc_s { ct_data* dyn_tree; int max_code; struct static_tree_desc_s* stat_desc; };

typedef struct internal_state {
    z_streamp strm;            // +0x00
    int    status;             // +0x04
    Bytef* pending_buf;        // +0x08
    ulg    pending_buf_size;   // +0x0c
    Bytef* pending_out;        // +0x10
    int    pending;            // +0x14
    int    wrap;               // +0x18
    Byte   data_type;          // +0x1c
    Byte   method;             // +0x1d
    int    last_flush;         // +0x20
    uInt   w_size;             // +0x24
    uInt   w_bits;             // +0x28
    uInt   w_mask;             // +0x2c
    Bytef* window;             // +0x30
    ulg    window_size;        // +0x34
    Posf*  prev;               // +0x38
    Posf*  head;               // +0x3c
    uInt   ins_h;              // +0x40
    uInt   hash_size;          // +0x44
    uInt   hash_bits;          // +0x48
    uInt   hash_mask;          // +0x4c
    uInt   hash_shift;         // +0x50
    long   block_start;        // +0x54
    uInt   match_length;       // +0x58
    IPos   prev_match;         // +0x5c
    int    match_available;    // +0x60
    uInt   strstart;           // +0x64
    uInt   match_start;        // +0x68
    uInt   lookahead;          // +0x6c
    uInt   prev_length;        // +0x70
    uInt   max_chain_length;   // +0x74
    uInt   max_lazy_match;     // +0x78
    int    level;              // +0x7c
    int    strategy;           // +0x80
    uInt   good_match;         // +0x84
    int    nice_match;         // +0x88
    struct ct_data_s dyn_ltree[573];   // +0x8c
    struct ct_data_s dyn_dtree[61];    // +0x980
    struct ct_data_s bl_tree[39];      // +0xa74
    struct tree_desc_s l_desc;         // +0xb10
    struct tree_desc_s d_desc;         // +0xb1c
    struct tree_desc_s bl_desc;        // +0xb28
    ush    bl_count[16];               // +0xb34
    int    heap[573];                  // +0xb54
    int    heap_len;                   // +0x1448
    int    heap_max;                   // +0x144c
    uch    depth[573];                 // +0x1450
    uch*   l_buf;                      // +0x1690
    uInt   lit_bufsize;                // +0x1694
    uInt   last_lit;                   // +0x1698
    ush*   d_buf;                      // +0x169c
    ulg    opt_len;                    // +0x16a0
    ulg    static_len;                 // +0x16a4
    uInt   matches;                    // +0x16a8
    int    last_eob_len;               // +0x16ac
    ush    bi_buf;                     // +0x16b0
    int    bi_valid;                   // +0x16b4
} deflate_state;                       // 0x16b8
typedef char charf;
typedef enum {
    need_more,      /* block not completed, need more input or more output */
    block_done,     /* block flush performed */
    finish_started, /* finish started, need only more output at next deflate */
    finish_done     /* finish done, accept no more input or output */
} block_state;

#define Z_NO_FLUSH      0
#define Z_FINISH        4
#define Z_NULL  0
#define MIN_MATCH 3
#define MAX_MATCH 258
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST(s) ((s)->w_size-MIN_LOOKAHEAD)

extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
#define zmemcpy memcpy

extern "C" void __cdecl _tr_flush_block(deflate_state* s, charf* buf, ulg stored_len, int eof);   // 0x00C057D0 (trees.c)
extern "C" void __cdecl fill_window(deflate_state* s);                                             // 0x00C01090

extern "C" {
static void flush_pending(z_streamp strm)                                                         // 0x00C00630
{
    unsigned len = strm->state->pending;

    if (len > strm->avail_out) len = strm->avail_out;
    if (len == 0) return;

    zmemcpy(strm->next_out, strm->state->pending_out, len);
    strm->next_out  += len;
    strm->state->pending_out  += len;
    strm->total_out += len;
    strm->avail_out  -= len;
    strm->state->pending -= len;
    if (strm->state->pending == 0) {
        strm->state->pending_out = strm->state->pending_buf;
    }
}
}

#define FLUSH_BLOCK_ONLY(s, eof) { \
   _tr_flush_block(s, (s->block_start >= 0L ? \
                   (charf *)&s->window[(unsigned)s->block_start] : \
                   (charf *)Z_NULL), \
                (ulg)((long)s->strstart - s->block_start), \
                (eof)); \
   s->block_start = s->strstart; \
   flush_pending(s->strm); \
}

#define FLUSH_BLOCK(s, eof) { \
   FLUSH_BLOCK_ONLY(s, eof); \
   if (s->strm->avail_out == 0) return (eof) ? finish_started : need_more; \
}

extern "C" block_state __cdecl deflate_stored(deflate_state* s, int flush)
{
    ulg max_block_size = 0xffff;
    ulg max_start;

    if (max_block_size > s->pending_buf_size - 5) {
        max_block_size = s->pending_buf_size - 5;
    }

    for (;;) {
        if (s->lookahead <= 1) {
            fill_window(s);
            if (s->lookahead == 0 && flush == Z_NO_FLUSH) return need_more;

            if (s->lookahead == 0) break;
        }
        s->strstart += s->lookahead;
        s->lookahead = 0;

        max_start = s->block_start + max_block_size;
        if (s->strstart == 0 || (ulg)s->strstart >= max_start) {
            s->lookahead = (uInt)(s->strstart - max_start);
            s->strstart = (uInt)max_start;
            FLUSH_BLOCK(s, 0);
        }
        if (s->strstart - (uInt)s->block_start >= MAX_DIST(s)) {
            FLUSH_BLOCK(s, 0);
        }
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return flush == Z_FINISH ? finish_done : block_done;
}