// read_buf(z_streamp strm, Bytef* buf, unsigned size) -- zlib 1.2.1 deflate.c read_buf: move up to size input bytes into buf, updating adler32 (zlib wrap) or crc32 (gzip wrap)
// Leaf: _read_buf
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C00D50, 119 bytes, a TU-local static (or API) of the same library file as the landed
//   row 0x00C01090. Shape (retry1): parked as an "LTCG custom register convention" gap; the landed host TU already holds this
//   function's real body inside `extern "C" { static __declspec(noinline) }` beside its real caller, and graded with this
//   leaf it matches retail as is (VC7.1 /O2 picks the private convention itself). The source is the host TU verbatim.
// TU host (landed row 0x00C01090): fill_window(deflate_state* s) -- zlib 1.2.1 deflate.c fill_window: slide the window and hash chains when strstart nears the end, then read more input and prime the hash
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

extern "C" uLong __cdecl z_adler32(uLong adler, const Bytef* buf, uInt len);   // 0x00C03CB0
extern "C" uLong __cdecl z_crc32(uLong crc, const Bytef* buf, uInt len);       // 0x00C05FD0
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

#define NIL 0
#define MIN_MATCH 3
#define MAX_MATCH 258
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST(s) ((s)->w_size-MIN_LOOKAHEAD)
#define UPDATE_HASH(s,h,c) (h = (((h)<<s->hash_shift) ^ (c)) & s->hash_mask)
#define zmemcpy memcpy

extern "C" {

static __declspec(noinline) int read_buf(z_streamp strm, Bytef* buf, unsigned size)
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

}   // extern "C"

extern "C" void __cdecl fill_window(deflate_state* s)
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