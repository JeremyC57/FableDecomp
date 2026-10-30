#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#endif
// longest_match(deflate_state* s, IPos cur_match) -- zlib 1.2.1 deflate.c longest_match: walk the hash chain from cur_match and return the length of the longest match (match_start set to its position)
// Leaf: _longest_match
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C00E60, 359 bytes. zlib 1.2.1 ("deflate 1.2.1 Copyright 1995-2003" in .rdata),
// built with Z_PREFIX (public entry points `_z_*`); longest_match is a deflate.c `local` (static) helper.
// Callers: deflate_fast 0x00C013C0 (call at 0x00C01479) and deflate_slow 0x00C01750 (call at 0x00C01820), both with
// s in esi and cur_match in eax, plain `ret`: the private register convention VC7.1 gives a TU-local static that has a
// visible caller in the same TU (no /GL). Here the caller is a reduced deflate_fast match step (not graded); the
// leaf is longest_match only. Callees: none.
// Identity: the deflate_state offsets read (prev_length +0x70, max_chain_length +0x74, good_match +0x84, nice_match
// +0x88, w_size +0x24, w_mask +0x2c, window +0x30, prev +0x38, strstart +0x64, match_start +0x68, lookahead +0x6c),
// MAX_DIST = w_size - 0x106, MAX_MATCH 0x102, the 8x unrolled compare loop and `chain_length >>= 2` are the 1.2.1 C
// longest_match (no ASMV, no UNALIGNED_OK) verbatim.
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

struct internal_state;
typedef struct z_stream_s z_stream;
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
    uInt   prev_length;        // +0x70
    uInt   max_chain_length;   // +0x74
    uInt   max_lazy_match;     // +0x78
    int    level;              // +0x7c
    int    strategy;           // +0x80
    uInt   good_match;         // +0x84
    int    nice_match;         // +0x88
} deflate_state;               // (tail omitted: longest_match touches nothing past +0x88)

#define NIL 0
#define MIN_MATCH 3
#define MAX_MATCH 258
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST(s) ((s)->w_size-MIN_LOOKAHEAD)
#define Z_HUFFMAN_ONLY 2

extern "C" {

static __declspec(noinline) uInt longest_match(deflate_state* s, IPos cur_match)
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

}   // extern "C"

static unsigned char g_win[1200];
static unsigned short g_prev[1024];

static void setup(deflate_state& s)
{
    std::memset(&s, 0, sizeof(s));
    s.w_size = 1024; s.w_mask = 1023; s.window = g_win; s.window_size = 1024; s.prev = g_prev;
    s.max_chain_length = 8; s.good_match = 100; s.nice_match = 258; s.lookahead = 300;
}

int main()
{
    deflate_state s;
    // window: "abcdefgh" repeated pattern pieces; current string at 600 matches the string at 100 for 20 bytes
    for (int i = 0; i < 1200; ++i) g_win[i] = (unsigned char)(i * 7 + 3);
    for (int i = 0; i < 20; ++i) g_win[100 + i] = g_win[600 + i];
    for (int i = 0; i < 6; ++i) g_win[300 + i] = g_win[600 + i];   // shorter 6-byte match at 300
    setup(s);
    s.strstart = 600; s.prev_length = 2;
    g_prev[300] = 100;                   // chain: 300 -> 100 -> 0 (stop: 0 <= limit 0; MAX_DIST 762 > strstart)
    g_prev[100] = 0;
    uInt len = longest_match(&s, 300);
    if (len != 20 || s.match_start != 100) { std::printf("00c00e60_TEST FAIL 1 %u %u\n", len, s.match_start); return 1; }
    // lookahead clamps the result
    setup(s); s.strstart = 600; s.prev_length = 2; s.lookahead = 10;
    len = longest_match(&s, 300);
    if (len != 10) { std::printf("00c00e60_TEST FAIL 2 %u\n", len); return 1; }
    // nice_match stops at the first long-enough match (300, length 6)
    setup(s); s.strstart = 600; s.prev_length = 2; s.nice_match = 5;
    s.match_start = 0;
    len = longest_match(&s, 300);
    if (len != 6 || s.match_start != 300) { std::printf("00c00e60_TEST FAIL 3 %u\n", len); return 1; }
    // no match better than prev_length: returns prev_length, match_start untouched
    setup(s); s.strstart = 600; s.prev_length = 30; s.match_start = 77;
    len = longest_match(&s, 300);
    if (len != 30 || s.match_start != 77) { std::printf("00c00e60_TEST FAIL 4 %u\n", len); return 1; }
    // full-length match caps at MAX_MATCH (258)
    for (int i = 0; i < 270; ++i) g_win[i] = g_win[600 + i] = 'z';
    setup(s); s.strstart = 600; s.prev_length = 2; g_prev[5] = 0;
    len = longest_match(&s, 5);
    if (len != 258 || s.match_start != 5) { std::printf("00c00e60_TEST FAIL 5 %u\n", len); return 1; }
    std::printf("00c00e60_TEST PASS\n");
    return 0;
}