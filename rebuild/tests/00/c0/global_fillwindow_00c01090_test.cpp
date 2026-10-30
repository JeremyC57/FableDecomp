#include <cstdio>
#include <cstring>
// fill_window(deflate_state* s) -- zlib 1.2.1 deflate.c fill_window: slide the window and hash chains when strstart nears the end, then read more input and prime the hash
// Leaf: _fill_window
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C01090, 269 bytes. zlib 1.2.1 ("deflate 1.2.1 Copyright 1995-2003" in .rdata).
// Callers: deflate_stored 0x00C011CB, deflate_fast 0x00C013DE, deflate_slow 0x00C0176B (all three call it with s on
// the stack). Callee: read_buf 0x00C00D50, the TU-local static, which VC7.1 gives a private register convention
// (strm in ebx, size in ecx, buf on the stack): its same-TU body is required for the caller to use it.
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
typedef voidpf ( *alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   ( *free_func)(voidpf opaque, voidpf address);

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
} deflate_state;               // (tail omitted: fill_window touches nothing past +0x6c)


static int g_adlerCalls = 0;
static uLong z_adler32(uLong adler, const Bytef* buf, uInt len) { ++g_adlerCalls; return adler + len; }
static uLong z_crc32(uLong crc, const Bytef* buf, uInt len) { return crc ^ len; }


#define NIL 0
#define MIN_MATCH 3
#define MAX_MATCH 258
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST(s) ((s)->w_size-MIN_LOOKAHEAD)
#define UPDATE_HASH(s,h,c) (h = (((h)<<s->hash_shift) ^ (c)) & s->hash_mask)
#define zmemcpy memcpy

static int read_buf(z_streamp strm, Bytef* buf, unsigned size)
{
    unsigned len = strm->avail_in;

    if (len > size) len = size;
    if (len == 0) return 0;

    strm->avail_in  -= len;

    if (strm->state->wrap == 1) {
        strm->adler = z_adler32(strm->adler, strm->next_in, len);
    }
    else if (strm->state->wrap == 2) {
        strm->adler = z_crc32(strm->adler, strm->next_in, len);
    }
    zmemcpy(buf, strm->next_in, len);
    strm->next_in  += len;
    strm->total_in += len;

    return (int)len;
}

static void fill_window(deflate_state* s)
{
    register unsigned n, m;
    register Posf *p;
    unsigned more;    /* Amount of free space at the end of the window. */
    uInt wsize = s->w_size;

    do {
        more = (unsigned)(s->window_size -(ulg)s->lookahead -(ulg)s->strstart);

        /* Deal with !@#$% 64K limit: sizeof(int) <= 2 is false on Win32, so only the slide remains */
        if (s->strstart >= wsize+MAX_DIST(s)) {

            zmemcpy(s->window, s->window+wsize, (unsigned)wsize);
            s->match_start -= wsize;
            s->strstart    -= wsize; /* we now have strstart >= MAX_DIST */
            s->block_start -= (long) wsize;

            n = s->hash_size;
            p = &s->head[n];
            do {
                m = *--p;
                *p = (Pos)(m >= wsize ? m-wsize : NIL);
            } while (--n);

            n = wsize;
            p = &s->prev[n];
            do {
                m = *--p;
                *p = (Pos)(m >= wsize ? m-wsize : NIL);
            } while (--n);
            more += wsize;
        }
        if (s->strm->avail_in == 0) return;

        n = read_buf(s->strm, s->window + s->strstart + s->lookahead, more);
        s->lookahead += n;

        /* Initialize the hash value now that we have some input: */
        if (s->lookahead >= MIN_MATCH) {
            s->ins_h = s->window[s->strstart];
            UPDATE_HASH(s, s->ins_h, s->window[s->strstart+1]);
        }

    } while (s->lookahead < MIN_LOOKAHEAD && s->strm->avail_in != 0);
}

int main()
{
    static Bytef window[64];
    static Posf prev[32];
    static Posf head[8];
    static Bytef input[200];
    for (int i = 0; i < 200; ++i) input[i] = (Bytef)(i + 1);
    z_stream strm; std::memset(&strm, 0, sizeof(strm));
    deflate_state s; std::memset(&s, 0, sizeof(s));
    strm.next_in = input; strm.avail_in = 10; strm.state = &s; strm.adler = 1;
    s.strm = &strm; s.wrap = 1; s.w_size = 32; s.window = window; s.window_size = 64;
    s.prev = prev; s.head = head; s.hash_size = 8; s.hash_mask = 0xff; s.hash_shift = 5;
    // 1) no slide: reads all 10 bytes, primes the hash, stops because input ran dry
    fill_window(&s);
    if (s.lookahead != 10 || strm.avail_in != 0 || strm.total_in != 10 || window[0] != 1 || window[9] != 10 ||
        s.ins_h != (((1u << 5) ^ 2u) & 0xff) || strm.adler != 11 || g_adlerCalls != 1)
    { std::printf("00c01090_TEST FAIL\n"); return 1; }
    // 2) no input left: the early return leaves lookahead alone (w_size 300 keeps the slide test false)
    s.w_size = 300; s.window_size = 0;
    strm.avail_in = 0; uInt la = s.lookahead;
    fill_window(&s);
    if (s.lookahead != la) { std::printf("00c01090_TEST FAIL\n"); return 1; }
    // 3) slide path with a consistent geometry: wsize 270 (MAX_DIST 8), strstart 280 >= 278
    static Bytef win2[600]; static Posf prev2[270]; static Posf head2[4];
    for (int i = 0; i < 600; ++i) win2[i] = (Bytef)i;
    for (int i = 0; i < 270; ++i) prev2[i] = (Posf)(i * 2);
    head2[0] = 5; head2[1] = 270; head2[2] = 300; head2[3] = 600;
    s.w_size = 270; s.window = win2; s.window_size = 540; s.prev = prev2; s.head = head2; s.hash_size = 4;
    s.strstart = 280; s.lookahead = 0; s.match_start = 275; s.block_start = 270;
    strm.next_in = input; strm.avail_in = 5; strm.total_in = 0; s.wrap = 0;
    fill_window(&s);
    if (s.strstart != 10 || s.match_start != 5 || s.block_start != 0 || head2[0] != 0 || head2[1] != 0 ||
        head2[2] != 30 || head2[3] != 330 || prev2[0] != 0 || prev2[200] != 130 || win2[0] != (Bytef)270 ||
        s.lookahead != 5 || win2[10] != 1 || win2[14] != 5)
    { std::printf("00c01090_TEST FAIL\n"); return 1; }
    std::printf("00c01090_TEST PASS\n");
    return 0;
}