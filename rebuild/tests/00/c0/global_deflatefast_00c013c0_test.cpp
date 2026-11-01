// Behaviour test for 0x00C013C0 deflate_fast (zlib 1.2.1): a repeating input is tallied as literals then back-references
#include <cstdio>
#include <cstring>
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
typedef voidpf (*alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (*free_func)(voidpf opaque, voidpf address);

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
#define Z_HUFFMAN_ONLY  2
#define Z_RLE           3
#define NIL 0
#define LITERALS 256
#define Freq fc.freq
#define max_insert_length max_lazy_match
#define Z_NULL  0
#define MIN_MATCH 3
#define MAX_MATCH 258
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST(s) ((s)->w_size-MIN_LOOKAHEAD)



#define zmemcpy memcpy

   // 0x00C057D0 (trees.c)
                                             // 0x00C01090

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


static uch _length_code[256]; static uch _dist_code[512];
static int gFlushes = 0; static int gFed = 0;
static unsigned char gWindow[65536 * 2]; static unsigned short gPrev[65536], gHead[65536];
static unsigned char gLbuf[0x4000]; static unsigned short gDbuf[0x4000]; static unsigned char gPending[0x10000];
static unsigned gLastLit = 0, gD0 = 99, gD8 = 0, gL0 = 0;
static void _tr_flush_block(deflate_state* s, charf*, ulg, int) { ++gFlushes; gLastLit = s->last_lit; gD0 = s->d_buf[0]; gD8 = 0; for (unsigned k = 0; k < s->last_lit; ++k) if (s->d_buf[k] == 8) ++gD8; gL0 = s->l_buf[0]; s->last_lit = 0; }
static void fill_window(deflate_state* s)
{
    if (gFed) return;
    gFed = 1;
    for (int i = 0; i < 600; ++i) gWindow[i] = (unsigned char)("abcdefgh"[i % 8]);
    s->lookahead = 600;
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

#define UPDATE_HASH(s,h,c) (h = (((h)<<s->hash_shift) ^ (c)) & s->hash_mask)
#define INSERT_STRING(s, str, match_head) \
   (UPDATE_HASH(s, s->ins_h, s->window[(str) + (MIN_MATCH-1)]), \
    match_head = s->prev[(str) & s->w_mask] = s->head[s->ins_h], \
    s->head[s->ins_h] = (Pos)(str))
   // 0x012AF590 (trees.h)
     // 0x012AF390 (trees.h)
#define d_code(dist) \
   ((dist) < 256 ? _dist_code[dist] : _dist_code[256+((dist)>>7)])
#define _tr_tally_lit(s, c, flush) \
  { uch cc = (c); \
    s->d_buf[s->last_lit] = 0; \
    s->l_buf[s->last_lit++] = cc; \
    s->dyn_ltree[cc].Freq++; \
    flush = (s->last_lit == s->lit_bufsize-1); \
   }
#define _tr_tally_dist(s, distance, length, flush) \
  { uch len = (length); \
    ush dist = (distance); \
    s->d_buf[s->last_lit] = dist; \
    s->l_buf[s->last_lit++] = len; \
    dist--; \
    s->dyn_ltree[_length_code[len]+LITERALS+1].Freq++; \
    s->dyn_dtree[d_code(dist)].Freq++; \
    flush = (s->last_lit == s->lit_bufsize-1); \
  }
static uInt longest_match(deflate_state* s, IPos cur_match)
{
    unsigned chain_length = s->max_chain_length;/* max hash chain length */
    register Bytef *scan = s->window + s->strstart; /* current string */
    register Bytef *match;                       /* matched string */
    register int len;                           /* length of current match */
    int best_len = s->prev_length;              /* best match length so far */
    int nice_match = s->nice_match;             /* stop if match long enough */
    IPos limit = s->strstart > (IPos)MAX_DIST(s) ?
        s->strstart - (IPos)MAX_DIST(s) : NIL;
    Posf *prev = s->prev;
    uInt wmask = s->w_mask;
    register Bytef *strend = s->window + s->strstart + MAX_MATCH;
    register Byte scan_end1  = scan[best_len-1];
    register Byte scan_end   = scan[best_len];

    if (s->prev_length >= s->good_match) {
        chain_length >>= 2;
    }
    if ((uInt)nice_match > s->lookahead) nice_match = s->lookahead;

    do {
        match = s->window + cur_match;

        if (match[best_len]   != scan_end  ||
            match[best_len-1] != scan_end1 ||
            *match            != *scan     ||
            *++match          != scan[1])      continue;

        scan += 2, match++;

        do {
        } while (*++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 scan < strend);

        len = MAX_MATCH - (int)(strend - scan);
        scan = strend - MAX_MATCH;

        if (len > best_len) {
            s->match_start = cur_match;
            best_len = len;
            if (len >= nice_match) break;
            scan_end1  = scan[best_len-1];
            scan_end   = scan[best_len];
        }
    } while ((cur_match = prev[cur_match & wmask]) > limit
             && --chain_length != 0);

    if ((uInt)best_len <= s->lookahead) return (uInt)best_len;
    return s->lookahead;
}
static uInt longest_match_fast(deflate_state* s, IPos cur_match)
{
    register Bytef *scan = s->window + s->strstart; /* current string */
    register Bytef *match;                       /* matched string */
    register int len;                           /* length of current match */
    register Bytef *strend = s->window + s->strstart + MAX_MATCH;

    match = s->window + cur_match;

    /* Return failure if the match length is less than 2: */
    if (match[0] != scan[0] || match[1] != scan[1]) return MIN_MATCH-1;

    scan += 2, match += 2;

    do {
    } while (*++scan == *++match && *++scan == *++match &&
             *++scan == *++match && *++scan == *++match &&
             *++scan == *++match && *++scan == *++match &&
             *++scan == *++match && *++scan == *++match &&
             scan < strend);

    len = MAX_MATCH - (int)(strend - scan);

    if (len < MIN_MATCH) return MIN_MATCH - 1;

    s->match_start = cur_match;
    return len <= s->lookahead ? len : s->lookahead;
}
block_state deflate_fast(deflate_state* s, int flush)
{
    IPos hash_head = NIL; /* head of the hash chain */
    int bflush;           /* set if current block must be flushed */

    for (;;) {
        if (s->lookahead < MIN_LOOKAHEAD) {
            fill_window(s);
            if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) {
                return need_more;
            }
            if (s->lookahead == 0) break; /* flush the current block */
        }

        if (s->lookahead >= MIN_MATCH) {
            INSERT_STRING(s, s->strstart, hash_head);
        }

        if (hash_head != NIL && s->strstart - hash_head <= MAX_DIST(s)) {
            if (s->strategy < Z_HUFFMAN_ONLY) {
                s->match_length = longest_match (s, hash_head);
            } else if (s->strategy == Z_RLE && s->strstart - hash_head == 1) {
                s->match_length = longest_match_fast (s, hash_head);
            }
        }
        if (s->match_length >= MIN_MATCH) {
            _tr_tally_dist(s, s->strstart - s->match_start,
                           s->match_length - MIN_MATCH, bflush);

            s->lookahead -= s->match_length;

            if (s->match_length <= s->max_insert_length &&
                s->lookahead >= MIN_MATCH) {
                s->match_length--; /* string at strstart already in table */
                do {
                    s->strstart++;
                    INSERT_STRING(s, s->strstart, hash_head);
                } while (--s->match_length != 0);
                s->strstart++;
            } else
            {
                s->strstart += s->match_length;
                s->match_length = 0;
                s->ins_h = s->window[s->strstart];
                UPDATE_HASH(s, s->ins_h, s->window[s->strstart+1]);
            }
        } else {
            _tr_tally_lit (s, s->window[s->strstart], bflush);
            s->lookahead--;
            s->strstart++;
        }
        if (bflush) FLUSH_BLOCK(s, 0);
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return flush == Z_FINISH ? finish_done : block_done;
}

int main()
{
    static z_stream strm; static deflate_state st; deflate_state* s = &st;
    std::memset(&strm, 0, sizeof(strm)); std::memset(&st, 0, sizeof(st));
    for (int i = 0; i < 256; ++i) { _length_code[i] = (uch)(i / 16); _dist_code[i] = (uch)(i / 32); _dist_code[256 + i] = (uch)(8 + i / 64); }
    unsigned char out[4096]; strm.next_out = out; strm.avail_out = sizeof(out); strm.state = &st;
    s->strm = &strm; s->window = gWindow; s->prev = gPrev; s->head = gHead; s->w_size = 0x8000; s->w_mask = 0x7fff;
    s->hash_bits = 15; s->hash_size = 1 << 15; s->hash_mask = s->hash_size - 1; s->hash_shift = 5;
    s->l_buf = gLbuf; s->d_buf = gDbuf; s->lit_bufsize = 0x4000; s->pending_buf = gPending; s->pending_out = gPending;
    s->max_chain_length = 4; s->max_lazy_match = 4; s->good_match = 4; s->nice_match = 8; s->strategy = 0; s->prev_length = 2;
    block_state r = deflate_fast(s, Z_FINISH);
    if (r != finish_done || gFlushes != 1 || gLastLit < 9 || gLastLit > 200 || gD0 != 0 || gL0 != 'a' || gD8 == 0 || s->strstart != 600) { std::printf("00c013c0_TEST FAIL\n"); return 1; }
    /* the first 8 bytes are literals, then distance-8 matches */
    std::memset(&st, 0, 0);
    std::printf("00c013c0_TEST PASS\n");
    return 0;
}